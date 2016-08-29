#include <pebble.h>
#include "perlin.h"
#include "watchface/view.h"
#include "watchface/app.h"
#include "service/settings.h"
#include "service/battery.h"
#include "service/connection.h"
#include "service/ticktimer.h"

void force_update(void) {

  // update on launch
  service_connection_service_update();
  service_battery_state_service_update();
  service_tick_timer_service_update();
  settings_reload_view(watchface_app());
  window_stack_push(watchface_app()->view->window, true); 

}

void handle_init(void) {

  init_watchface_app();
  // handlers
  service_battery_state_service_subscribe();
  service_connection_service_subscribe();
  service_tick_timer_service_subscribe();
  
  
  force_update();
}

void handle_deinit(void) {

  deinit_watchface_app(watchface_app());
  service_tick_timer_service_unsubscribe();
  service_connection_service_unsubscribe();
  service_battery_state_service_unsubscribe();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
