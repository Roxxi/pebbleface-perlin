#include <pebble.h>
#include "perlin2.h"
#include "watchface_view.h"
#include "background.h"
#include "resource_broker.h"


#if DEV_MODE
  static const int TEXT_LAYERS=5;
#else
  static const int TEXT_LAYERS=4;
#endif

typedef struct {
  TextLayerId    id;
  GRect          text_area;
  GColor         text_color;
  GColor         background_color;
  GFont          text_font;
  GTextAlignment text_alignment;
} TextLayerConfig ;

#ifdef PBL_PLATFORM_CHALK
static TextLayerConfig text_layer_configs[TEXT_LAYERS] = {
  { TOP,    GRect(0, 11, 178, 18),  GColorBlack, GColorWhite, NULL, GTextAlignmentCenter },
  { HOUR,   GRect(0, 16, 182, 64),  GColorBlack, GColorWhite, NULL, GTextAlignmentCenter },
  { MIN,    GRect(0, 73, 182, 64),  GColorBlack, GColorWhite, NULL, GTextAlignmentCenter },
  { BOTTOM, GRect(0, 141, 178, 18), GColorBlack, GColorWhite, NULL, GTextAlignmentCenter }
  #if DEV_MODE
    , // TODO Add Layer for DEV, don't get rid of comma
  #endif
};
#else
static TextLayerConfig text_layer_configs[TEXT_LAYERS] = {
  { TOP,    GRect(50, 5, 36, 18),    GColorBlack, GColorWhite, NULL, GTextAlignmentCenter },
  { HOUR,   GRect(0, 15, 146, 64),   GColorBlack, GColorWhite, NULL, GTextAlignmentCenter },
  { MIN,    GRect(0, 72, 146, 64),   GColorBlack, GColorWhite, NULL, GTextAlignmentCenter },
  { BOTTOM, GRect(22, 145, 106, 18), GColorBlack, GColorWhite, NULL, GTextAlignmentCenter }
  #if DEV_MODE
    , // TODO Add Layer for DEV, don't get rid of comma
  #endif
};
#endif  


GFont get_font_for_section (TextLayerId id){
  if (id == TOP || id == BOTTOM || id == DEV){
    return get_date_font();
  } else if (id == HOUR || id == MIN) {
    return get_time_font();
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Did not find font for section %d", id);
    return fonts_get_system_font(FONT_KEY_GOTHIC_09);
  }
}

TextLayer* create_text_layer(TextLayerConfig tlc) {
  TextLayer* layer; 
  GFont custom_font;

  layer = text_layer_create(tlc.text_area);
  text_layer_set_text_color(layer, tlc.text_color);
  text_layer_set_background_color(layer, tlc.background_color);
  text_layer_set_text_alignment(layer, tlc.text_alignment);
  // We're providing our own fonts, so we need to dynamic load
  if (tlc.text_font == NULL) {
    custom_font = get_font_for_section(tlc.id);
    text_layer_set_font(layer, custom_font);
  } else {
    text_layer_set_font(layer, tlc.text_font);
  }
  return layer;
}

WatchfaceView* init_window(WatchfaceView* wfv){
  wfv->window = window_create();
  wfv->window_layer = window_get_root_layer(wfv->window);
  window_set_background_color(wfv->window, GColorWhite);
  return wfv;
}

WatchfaceView* init_text_layers(WatchfaceView* wfv){
  TextLayerConfig tlc;
  TextLayerId id;

  wfv->text_layers = malloc(TEXT_LAYERS * sizeof(TextLayer*));
  for(int i = 0; i < TEXT_LAYERS; i++) {
    tlc = text_layer_configs[i];
    id = tlc.id;
    wfv->text_layers[id] = create_text_layer(tlc);
  }  
  return wfv;
}


WatchfaceView* init_watchface_view() {

  WatchfaceView* wfv;

  wfv = malloc(sizeof(WatchfaceView));
  init_window(wfv);
  init_background_layer( wfv->window_layer );
  init_text_layers(wfv); 
  // composing layers
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[TOP]) );
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[MIN]) );
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[HOUR]) );
  layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[BOTTOM]) );
      
  return wfv;
}

void deinit_watchface_view(WatchfaceView* wfv){
  deinit_background_layer();
  deinit_fonts();
  for(int i = 0; i < TEXT_LAYERS; i++) {
    text_layer_destroy(wfv->text_layers[i]);
  }  
  free(wfv);
}
