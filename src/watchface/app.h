#pragma once

#include <pebble.h>
#include "perlin.h"
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

// Update screen
void watchface_app_update(WatchfaceApp* app);

// Handlers
void settings_reload_view(WatchfaceApp* app);
void app_handle_charge_percent(WatchfaceApp* app, int charge_percent, bool is_charging);
void app_handle_bluetooth_connection(WatchfaceApp* app, bool is_connected);
void app_handle_hour_tick(WatchfaceApp* app, struct tm *tick_time, TimeUnits units_changed);
void app_handle_minute_tick(WatchfaceApp* app, struct tm *tick_time, TimeUnits units_changed);