#include <pebble.h>
#include "perlin2.h"
#include "watchface_view.h"
#include "background.h"
#include "resource_broker.h"


#if DEV_MODE
  #define NUM_TEXT_LAYERS 4
#else
  #define NUM_TEXT_LAYERS 4
#endif

typedef struct {
  TextLayerId    id;
  GRect          text_area;
  GColor         text_color;
  GColor         background_color;
  GFont          text_font;
  GTextAlignment text_alignment;
} TextLayerConfig ;





TextLayerConfig*  init_text_layer_configs(){
  TextLayerConfig*  tlcs = malloc(sizeof(TextLayerConfig) * NUM_TEXT_LAYERS);
  #ifdef PBL_PLATFORM_CHALK // Pebble Time Round
    tlcs[0] = (TextLayerConfig) { TOP,    GRect(0, 11, 178, 18),  GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    tlcs[1] = (TextLayerConfig) { HOUR,   GRect(0, 16, 182, 64),  GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[2] = (TextLayerConfig) { MIN,    GRect(0, 73, 182, 64),  GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[3] = (TextLayerConfig) { BOTTOM, GRect(0, 141, 178, 18), GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    #if DEV_MODE
      //, TODO implement dev indicator
    #endif
  #else // Pebble Time 
    tlcs[0] = (TextLayerConfig) { TOP,    GRect(50, 5, 36, 18),    GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    tlcs[1] =(TextLayerConfig)  { HOUR,   GRect(0, 15, 146, 64),   GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[2] =(TextLayerConfig)  { MIN,    GRect(0, 72, 146, 64),   GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[3] =(TextLayerConfig)  { BOTTOM, GRect(22, 145, 106, 18), GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    #if DEV_MODE
      //, TODO implement dev indicator
    #endif
  #endif
  return tlcs;
}


TextLayer* create_text_layer(TextLayerConfig tlc) {
  TextLayer* layer; 

  layer = text_layer_create(tlc.text_area);
  text_layer_set_text_color(layer, tlc.text_color);
  text_layer_set_background_color(layer, tlc.background_color);
  text_layer_set_text_alignment(layer, tlc.text_alignment);
  text_layer_set_font(layer, tlc.text_font);
  return layer;
}

WatchfaceView* init_window(WatchfaceView* wfv){
  wfv->window = window_create();
  wfv->window_layer = window_get_root_layer(wfv->window);
  window_set_background_color(wfv->window, GColorWhite);
  wfv->background = init_background(wfv->window_layer);
  return wfv;
}

void deinit_window(WatchfaceView* wfv){
  window_destroy(wfv->window);
  deinit_background(wfv->background);
}


WatchfaceView* init_text_layers(WatchfaceView* wfv){
  TextLayerConfig tlc;
  TextLayerConfig *text_layer_configs;
  TextLayerId id;

  text_layer_configs = init_text_layer_configs();
  wfv->text_layers = malloc(NUM_TEXT_LAYERS * sizeof( TextLayer* ) );
  for(int i = 0; i < NUM_TEXT_LAYERS; i++) {
    tlc = text_layer_configs[i];
    id = tlc.id;
    wfv->text_layers[id] = create_text_layer(tlc);
  }  
  return wfv;
}

void deinit_text_layers(WatchfaceView* wfv){
  for(int i = 0; i < NUM_TEXT_LAYERS; i++) {
    text_layer_destroy(wfv->text_layers[i]);
  }  
}

void attach_layers(WatchfaceView * wfv){
  layer_add_child( wfv->window_layer, 
                   bitmap_layer_get_layer( wfv->background->layer ) );
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[TOP]) );
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[MIN]) );
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[HOUR]) );
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[BOTTOM]) );
}


WatchfaceView* init_watchface_view() {

  WatchfaceView* wfv;

  wfv = malloc(sizeof(WatchfaceView));
  init_window(wfv);
  init_text_layers(wfv); 
  attach_layers(wfv);
      
  return wfv;
}



void deinit_watchface_view(WatchfaceView* wfv){
  deinit_text_layers(wfv);
  deinit_window(wfv);
  deinit_fonts();
  free(wfv);
}

void random_background(WatchfaceView* wfv){
  set_random_background(wfv->background);
}
