#pragma once

#include <pebble.h>
#include "watchface/background.h"

typedef enum {  TOP, HOUR, MIN, BOTTOM, DEV } TextLayerId ;

typedef struct {
  Window *window;
  Layer *window_layer;
  WatchfaceBackground *background;
  TextLayer **text_layers;
  
} WatchfaceView;

WatchfaceView* init_watchface_view();
void deinit_watchface_view(WatchfaceView*);

// TODO prefix view
WatchfaceView* random_background(WatchfaceView*);

WatchfaceView* view_bottom_text_show(WatchfaceView* view);
WatchfaceView* view_bottom_text_hide(WatchfaceView* view);
WatchfaceView* view_bottom_text_update(WatchfaceView* view, char* text);


WatchfaceView* view_top_text_show(WatchfaceView* view);
WatchfaceView* view_top_text_hide(WatchfaceView* view);
WatchfaceView* view_top_text_update(WatchfaceView*, int percent, bool is_charging);





