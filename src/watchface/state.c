#include <pebble.h>
#include "perlin.h"
#include "watchface/state.h"
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
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "{ 'bluetoothview':%d, 'hourlyvibe':%d, 'randomwallpaper':%d, 'showdate':%d, 'showbatt':%d }",
          state->bluetoothvibe, state->hourlyvibe, state->randomwallpaper, state->showdate, state->showbatt);
  return state;
}


void deinit_watchface_state(WatchfaceState* state){
  free(state);
}