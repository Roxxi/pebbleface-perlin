#pragma once

#include <pebble.h>
#include "watchface/view.h"
#include "watchface/state.h"

typedef struct {
  AppSync sync;
  uint8_t* sync_buffer;
  WatchfaceState* state;
  WatchfaceView* view;
} WatchfaceApp;

// Constructor
WatchfaceApp* init_watchface_app();

// Destructor
void deinit_watchface_app(WatchfaceApp* app);

// Singleton for service intergration
WatchfaceApp* watchface_app();

// Handlers
void settings_reload_view(WatchfaceApp* app);
void app_update_charge_percent(WatchfaceApp* app, int charge_percent, bool is_charging);