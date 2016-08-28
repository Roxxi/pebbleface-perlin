#include <pebble.h>
#include "perlin.h"
#include "watchface/view.h"
#include "watchface/app.h"
#include "service/settings.h"
#include "service/battery.h"
#include "service/connection.h"



static WatchfaceApp* app; // The App, in all it's glory


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





void update_time(struct tm *tick_time) {

  static char h_time_text[] = "00";
  static char m_time_text[] = "00";
  static char date_text[] = "XXX XXX 00XX XXX";

  char *h_time_format;
  int new_cur_day = tick_time->tm_year*1000 + tick_time->tm_yday;

  if (new_cur_day != state_read_current_day(app->state)) {
    state_update_current_day(app->state, new_cur_day);
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
  text_layer_set_text(app->view->text_layers[BOTTOM], date_text);
  } // end if
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

void hourvibe (struct tm *tick_time) {

  if(state_read_hourly_vibe(app->state)) {
	  //vibe!
	  vibes_short_pulse();
  }
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);

  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Minute Updates");

  if (units_changed & HOUR_UNIT) {
    hourvibe(tick_time);
    random_background(app->view);
  }
#if DEV_MODE == 1
  if (units_changed & MINUTE_UNIT) {
    APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Updating Background on the Minute");
    log_mem_stats();
    random_background(app->view);  // used for testing
    log_mem_stats();
  }
  text_layer_set_text(app->view->text_layers[DEV], "DEV");

#endif

}

void force_update(void) {

  // update on launch
  service_connection_service_update();
  service_battery_state_service_update();
  time_t now = time(NULL);
  update_time(localtime(&now));
  settings_reload_view(app);
  window_stack_push(app->view->window, true); 

}

void handle_init(void) {

  app=init_watchface_app();
  // handlers
  service_battery_state_service_subscribe();
  service_connection_service_subscribe();
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  
  
  force_update();
}

void handle_deinit(void) {

  deinit_watchface_app(app);
  tick_timer_service_unsubscribe();
  service_connection_service_unsubscribe();
  service_battery_state_service_unsubscribe();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
