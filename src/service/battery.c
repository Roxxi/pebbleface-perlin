#include <pebble.h>
#include "service/battery.h"
#include "watchface/app.h"


void battery_charge_state_handler(BatteryChargeState charge_state) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling battery charge state update");
  app_handle_charge_percent(watchface_app(), 
                            charge_state.charge_percent, 
                            charge_state.is_charging);
}

void service_battery_state_service_subscribe(){
  battery_state_service_subscribe(battery_charge_state_handler);
}

void service_battery_state_service_unsubscribe(){
  battery_state_service_unsubscribe();
}

void service_battery_state_service_update(){
  battery_charge_state_handler(battery_state_service_peek());
}