#pragma once
#include <pebble.h>

typedef struct {
  bool bluetooth_vibe;
  bool hourly_vibe;
  bool random_background;
  bool show_date;
  bool show_battery;
  bool app_started; 
  int current_day;
  int charge_percent; 

} WatchfaceState;

// Constructor
WatchfaceState* init_watchface_state();

// Desctructor
void deinit_watchface_state(WatchfaceState*);

/*******  Individual Settings *******/

bool state_read_bluetooth_vibe(WatchfaceState* state);
void state_update_bluetooth_vibe(WatchfaceState* state, bool bluetooth_vibe); 

bool state_read_hourly_vibe(WatchfaceState* state);
void state_update_hourly_vibe(WatchfaceState* state, bool hourly_vibe);

bool state_read_random_background(WatchfaceState* state);
void state_update_random_background(WatchfaceState* state, bool random_background);

bool state_read_show_date(WatchfaceState* state);
void state_update_show_date(WatchfaceState* state, bool show_date);

bool state_read_show_battery(WatchfaceState* state);
void state_update_show_battery(WatchfaceState* state, bool show_battery);

int state_read_current_day(WatchfaceState* state);
void state_update_current_day(WatchfaceState* state, int current_day);

int state_read_charge_percernt(WatchfaceState* state);
void state_update_charge_percernt(WatchfaceState* state, int charge_percernt);

// Synchronize Memory <=> Persistent Storage
void state_load_settings(WatchfaceState* state);
void state_persist_settings(WatchfaceState* state);

// Debugging
void log_state(WatchfaceState* state);




