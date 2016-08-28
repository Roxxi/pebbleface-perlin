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

WatchfaceApp* init_watchface_app();
void deinit_watchface_app(WatchfaceApp* app);

void settings_reload_view(WatchfaceApp* app);
