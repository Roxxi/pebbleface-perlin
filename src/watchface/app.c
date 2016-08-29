#include <pebble.h>
#include "watchface/app.h"
#include "watchface/view.h"
#include "watchface/state.h"
#include "service/settings.h"
#include "service/battery.h"
#include "service/connection.h"
#include "service/ticktimer.h"

#define SYNC_BUFFER_SIZE 256

WatchfaceApp* init_watchface_app(){
  WatchfaceApp* app;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting App Initialization");

  app = malloc(sizeof(WatchfaceApp));

  app->sync_buffer = malloc(sizeof(uint8_t) * SYNC_BUFFER_SIZE);
  app_sync_init(&(app->sync), app->sync_buffer, SYNC_BUFFER_SIZE, NULL, 0,	NULL, NULL, app);
  app_message_register_inbox_received(settings_inbox_received_handler);
  app_message_open(SYNC_BUFFER_SIZE, SYNC_BUFFER_SIZE); 

  app->state = init_watchface_state();
  app->view = init_watchface_view();

  // service handler callbacks
  service_battery_state_service_subscribe(app);
  service_connection_service_subscribe(app);
  service_tick_timer_service_subscribe(app);
    
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Completed App Initialization");

  return app;
  
}

// n.b. - reverse order of constructor
void deinit_watchface_app(WatchfaceApp* app){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting App Deallocation");

  // deregister service handlers
  service_tick_timer_service_unsubscribe();
  service_connection_service_unsubscribe();
  service_battery_state_service_unsubscribe();
  // free allocated memory
  deinit_watchface_view(app->view);
  deinit_watchface_state(app->state);
  app_sync_deinit(&(app->sync));
  free(app->sync_buffer);
  free(app);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Completed App Deallocation");

}

void watchface_app_update(WatchfaceApp* app){
  service_connection_service_update();
  service_battery_state_service_update();
  service_tick_timer_service_update();
  app_handle_top_text_toggle(app);
  app_handle_bottom_text_toggle(app);
  app_handle_random_background(app); 
  window_stack_push(app->view->window, true); 
}

void app_handle_apply_settings(WatchfaceApp* app){
  // Make sure to sync the settings with app state once we've loaded
  state_persist_settings(app->state); 
  // refresh view that settings control
  app_handle_top_text_toggle(app);
  app_handle_bottom_text_toggle(app);
  app_handle_random_background(app);
}

void app_handle_top_text_toggle(WatchfaceApp* app){
  if (state_read_show_battery(app->state)) {
    view_top_text_show(app->view);
  } else { 
    view_top_text_hide(app->view);
  }
}

void app_handle_bottom_text_toggle(WatchfaceApp* app) {
  if (state_read_show_date(app->state)) {
    view_bottom_text_show(app->view);
  } else {
    view_bottom_text_hide(app->view);
  }
}

void app_handle_random_background(WatchfaceApp* app){
  if (state_read_random_background(app->state)) {
     random_background(app->view);
   }
}

void app_handle_charge_percent(WatchfaceApp* app, int charge_percent, bool is_charging){
  APP_LOG(APP_LOG_LEVEL_DEBUG, 
          "Updating Charge Percent: pct %d, is_charging %d",
          charge_percent, is_charging);
  state_update_charge_percent(app->state, charge_percent);
  view_top_text_update(app->view, charge_percent, is_charging); 
}


void app_handle_bluetooth_connection(WatchfaceApp* app, bool is_connected){
  bool notify_user;

  notify_user = state_read_bluetooth_vibe(app->state);
  APP_LOG(APP_LOG_LEVEL_DEBUG, 
          "Bluetooth connection state: %d, notify_user: %d", 
          is_connected, notify_user);  
  if (!is_connected && notify_user) {
    vibes_long_pulse(); //vibe!
  }
}

/************ Hour Tick Handler **************/

// Hourly Bell

void hourly_vibe(WatchfaceApp* app){
  bool notify_user;

  notify_user = state_read_hourly_vibe(app->state);
  APP_LOG(APP_LOG_LEVEL_DEBUG, 
          "Hourly Bell notify_user: %d", 
           notify_user);  
  if (notify_user) {
    vibes_short_pulse(); //vibe!
  }
}

void app_handle_hour_tick(WatchfaceApp* app, struct tm *tick_time, TimeUnits units_changed){
  if (units_changed & HOUR_UNIT) { 
    hourly_vibe(app);
    random_background(app->view);
  }
}

/************ Minute Tick Handler **************/

char* formatted_current_date_text(WatchfaceApp* app, struct tm *tick_time) {
  static char date_text[] = "XXX XXX 00XX XXX";
  switch(tick_time->tm_mday) {
    case 1 :
    case 21 :
  	case 31 :
  	  strftime(date_text, sizeof(date_text), "%a, %est %b", tick_time);
      break;
    case 2 :
    case 22 :
      strftime(date_text, sizeof(date_text), "%a, %end %b", tick_time);
      break;
    case 3 :
    case 23 :
      strftime(date_text, sizeof(date_text), "%a, %erd %b", tick_time);
      break;
    default :
      strftime(date_text, sizeof(date_text), "%a, %eth %b", tick_time);
      break;
  } // end switch
  return date_text;
}

void update_date(WatchfaceApp* app, struct tm *tick_time) {
  char* date_text;
  int new_cur_day, prev_cur_day;

  new_cur_day = tick_time->tm_year*1000 + tick_time->tm_yday;
  prev_cur_day = state_read_current_day(app->state);

  if (new_cur_day != prev_cur_day) {
      state_update_current_day(app->state, new_cur_day);
      date_text = formatted_current_date_text(app, tick_time);
      view_bottom_text_update(app->view, date_text);
  }
}

void update_time(WatchfaceApp* app, struct tm *tick_time) {

  static char h_time_text[] = "00";
  static char m_time_text[] = "00";
  char *h_time_format;
  
  if (clock_is_24h_style()) {
    h_time_format = "%H";
  } else {
    h_time_format = "%I";
  }
  strftime(h_time_text, sizeof(h_time_text), h_time_format, tick_time);
  text_layer_set_text(app->view->text_layers[HOUR], h_time_text);

  strftime(m_time_text, sizeof(m_time_text), "%M", tick_time);
  text_layer_set_text(app->view->text_layers[MIN], m_time_text);
}


#if DEV_MODE == 1

void log_mem_stats(){
  size_t used, free, total;
  size_t pct;
  free = heap_bytes_free();
  used = heap_bytes_used();
  total = used + free;
  pct = used * 100 / total;
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "heap_bytes_free: %zu", free);
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "heap_bytes_used: %zu", used );
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "total_heap: %zu", total);
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "percent_used: %zu", pct);
}

void minute_test_debug(WatchfaceApp* app, struct tm *tick_time){
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Updating Background on the Minute");
  log_mem_stats();
  app_handle_random_background(app);
  log_mem_stats();
  text_layer_set_text(app->view->text_layers[DEV], "DEV");
}
#endif

void app_handle_minute_tick(WatchfaceApp* app, struct tm *tick_time, TimeUnits units_changed ){
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Minute Updates");

  if (units_changed & HOUR_UNIT) {
    app_handle_hour_tick(app, tick_time, units_changed);
  }
  if (units_changed & MINUTE_UNIT) {
    // n.b. Why is date updated on the minute?
    // A traveller may land in a different locale on the other side of
    // international dateline. The watch should update the date immediately.
    update_time(app, tick_time);
    update_date(app, tick_time);
    #if DEV_MODE == 1
    minute_test_debug(app, tick_time);
    #endif
  } 
}




