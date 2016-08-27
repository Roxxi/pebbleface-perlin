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

WatchfaceView* random_background(WatchfaceView*);

WatchfaceView* date_text_show(WatchfaceView*);
WatchfaceView* date_text_hide(WatchfaceView*);

WatchfaceView* battery_text_show(WatchfaceView*);
WatchfaceView* battery_text_hide(WatchfaceView*);





