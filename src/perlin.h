#pragma once

#include <pebble.h>
#include "watchface_view.h"
#include "watchface_state.h"

// set to 0 to for production builds
#define DEV_MODE 1

typedef struct {
  AppSync sync;
  uint8_t* sync_buffer;
  WatchfaceState* state;
  WatchfaceView* view;
} WatchfaceApp;
