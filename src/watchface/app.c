#include <pebble.h>
#include "watchface/app.h"
#include "watchface/view.h"
#include "watchface/state.h"
#include "service/settings.h"

#define SYNC_BUFFER_SIZE 256

static WatchfaceApp* the_app;

WatchfaceApp* init_watchface_app(){
  WatchfaceApp* app;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting App Initialization");

  app = malloc(sizeof(WatchfaceApp));
  //  app->sync no initializer?
  app->sync_buffer = malloc(sizeof(uint8_t) * SYNC_BUFFER_SIZE);
  app->state = init_watchface_state();
  app->view = init_watchface_view();
  app_sync_init(&(app->sync), app->sync_buffer, SYNC_BUFFER_SIZE, NULL, 0,	NULL, NULL, app);
  app_message_register_inbox_received(settings_inbox_received_handler);
  app_message_open(SYNC_BUFFER_SIZE, SYNC_BUFFER_SIZE); 

  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Completed App Initialization");
  
  // TODO get rid of singleton integration
  // This helps with service integration for now
  the_app = app; 
  return app;
  
}

void deinit_watchface_app(WatchfaceApp* app){
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting App Deallocation");

  free(app->sync_buffer);
  app_sync_deinit(&(app->sync));
  deinit_watchface_state(app->state);
  deinit_watchface_view(app->view);
  free(app);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Completed App Deallocation");

}

WatchfaceApp* watchface_app(){
  return the_app;
}

void settings_reload_view(WatchfaceApp* app){

  if (state_read_show_battery(app->state)) {
    battery_text_show(app->view);
  } else { 
    battery_text_hide(app->view);
  }
  
  if (state_read_show_date(app->state)) {
    date_text_show(app->view);
  } else {
    date_text_hide(app->view);
  }
 
   if (state_read_random_background(app->state)) {
     random_background(app->view);
   }
}

void app_update_charge_percent(WatchfaceApp* app, int charge_percent, bool is_charging){
  APP_LOG(APP_LOG_LEVEL_DEBUG, 
          "Updating Charge Percent: pct %d, is_charging %d",
          charge_percent, is_charging);
  state_update_charge_percent(app->state, charge_percent);
  view_battery_text_update(app->view, charge_percent, is_charging); 
}


void app_update_bluetooth_connection(WatchfaceApp* app, bool is_connected){
  bool notify_user;

  notify_user = state_read_bluetooth_vibe(app->state);
  APP_LOG(APP_LOG_LEVEL_DEBUG, 
          "Bluetooth connection state: %d, notify_user: %d", 
          is_connected, notify_user);  
  if (!is_connected && notify_user) {
    vibes_long_pulse(); //vibe!
  }
}




