#include <pebble.h>
#include "service/battery_service.h"
#include "watchface/app.h"


void battery_charge_state_callback(BatteryChargeState charge_state) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling battery charge state update");
  app_update_charge_percent(watchface_app(), 
                            charge_state.charge_percent, 
                            charge_state.is_charging);
}