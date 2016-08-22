#pragma once

#include <pebble.h>
#include "background.h"

typedef enum {  TOP, HOUR, MIN, BOTTOM, DEV } TextLayerId ;

typedef struct {
  Window *window;
  Layer *window_layer;
  WatchfaceBackground *background;
  TextLayer **text_layers;
  
} WatchfaceView;

WatchfaceView* init_watchface_view();
void deinit_watchface_view(WatchfaceView*);
void random_background(WatchfaceView*);

