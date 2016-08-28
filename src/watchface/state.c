#include <pebble.h>
#include "perlin.h"
#include "watchface_state.h"
#include "settings.h"


WatchfaceState* init_watchface_state(){
  WatchfaceState* state = malloc(sizeof(WatchfaceState));

  state->bluetoothvibe = setting_read_bluetooth();
  state->hourlyvibe = setting_read_hourlyvibe();
  state->randomwallpaper = setting_read_randomwallpaper();
  state->showdate = setting_read_showdate();
  state->showbatt = setting_read_showbatt();
  state->cur_day = -1;
  state->charge_percent = 0;
  
  return state;
}


void deinit_watchface_state(WatchfaceState* state){
  free(state);
}