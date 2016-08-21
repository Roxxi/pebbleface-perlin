#pragma once

#include <pebble.h>

typedef enum {  TOP, HOUR, MIN, BOTTOM, DEV } TextLayerId ;

typedef struct {
  Window *window;
  Layer *window_layer;
  TextLayer **text_layers;
  
} WatchfaceView;

WatchfaceView* init_watchface_view();
void deinit_watchface_view(WatchfaceView*);

