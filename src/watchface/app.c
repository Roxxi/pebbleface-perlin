#include <pebble.h>
#include "watchface/app.h"
#include "settings.h"
#include "watchface/view.h"
#include "watchface/state.h"

#define SYNC_BUFFER_SIZE 256



WatchfaceApp* init_watchface_app(){
  WatchfaceApp* app;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting App Initialization");

  app = malloc(sizeof(WatchfaceApp));
  //  app->sync no initializer?
  app->sync_buffer = malloc(sizeof(uint8_t) * SYNC_BUFFER_SIZE);
  app->state = init_watchface_state();
  app->view = init_watchface_view();
  app_sync_init(&(app->sync), app->sync_buffer, SYNC_BUFFER_SIZE, NULL, 0,	NULL, NULL, app);
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(SYNC_BUFFER_SIZE, SYNC_BUFFER_SIZE); 

  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Completed App Initialization");
  
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

void settings_reload_view(WatchfaceApp* app){
  app->state->showbatt ? battery_text_show(app->view) : battery_text_hide(app->view);
  app->state->showdate ? date_text_show(app->view) : date_text_hide(app->view);
  if(app->state->randomwallpaper) random_background(app->view);
}


