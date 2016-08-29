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
WatchfaceApp* watchface_app_init();

// Destructor
void watchface_app_deinit(WatchfaceApp* app);

// Bootstrap
void watchface_app_bootstrap(WatchfaceApp* app);

// Handlers
void app_handle_apply_settings(WatchfaceApp* app);
void app_handle_top_text_toggle(WatchfaceApp* app);
void app_handle_bottom_text_toggle(WatchfaceApp* app);
void app_handle_random_background(WatchfaceApp* app);
void app_handle_charge_percent(WatchfaceApp* app, int charge_percent, bool is_charging);
void app_handle_bluetooth_connection(WatchfaceApp* app, bool is_connected);
void app_handle_hour_tick(WatchfaceApp* app, struct tm *tick_time, TimeUnits units_changed);
void app_handle_minute_tick(WatchfaceApp* app, struct tm *tick_time, TimeUnits units_changed);