#pragma once
#include <pebble.h>

typedef struct {
  bool bluetoothvibe;
  bool hourlyvibe;
  bool randomwallpaper;
  bool showdate;
  bool showbatt;
  bool app_started; 
  int cur_day;
  int charge_percent; 

} WatchfaceState;

WatchfaceState* init_watchface_state();
void deinit_watchface_state(WatchfaceState*);

void sync_settings_state(WatchfaceState* state);
