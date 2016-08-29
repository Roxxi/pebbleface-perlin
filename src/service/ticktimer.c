#include <pebble.h>
#include "service/ticktimer.h"
#include "watchface/app.h"

static WatchfaceApp* app;

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  app_handle_minute_tick(app, tick_time, units_changed);
  
}

void service_tick_timer_service_subscribe(WatchfaceApp* a){
  app = a;
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

void service_tick_timer_service_unsubscribe(){
  tick_timer_service_unsubscribe();
}

void service_tick_timer_service_update(){
  time_t now = time(NULL);
  handle_minute_tick(localtime(&now), MINUTE_UNIT);
}

