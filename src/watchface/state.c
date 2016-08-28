#include <pebble.h>
#include "perlin.h"
#include "watchface/state.h"
#include "settings.h"


void sync_settings_state(WatchfaceState* state){
  state->bluetoothvibe = setting_read_bluetooth();
  state->hourlyvibe = setting_read_hourlyvibe();
  state->randomwallpaper = setting_read_randomwallpaper();
  state->showdate = setting_read_showdate();
  state->showbatt = setting_read_showbatt();
}



WatchfaceState* init_watchface_state(){
  WatchfaceState* state = malloc(sizeof(WatchfaceState));

  sync_settings_state(state);
  state->cur_day = -1;
  state->charge_percent = 0;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, 
          "{ 'bluetoothview':%d, 'hourlyvibe':%d, 'randomwallpaper':%d, 'showdate':%d, 'showbatt':%d, 'cur_day':%d, 'charge_percent': %d }",
          state->bluetoothvibe, 
          state->hourlyvibe, 
          state->randomwallpaper, 
          state->showdate, 
          state->showbatt, 
          state->cur_day, 
          state->charge_percent);
  return state;
}


void deinit_watchface_state(WatchfaceState* state){
  free(state);
}

