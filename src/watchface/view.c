#include <pebble.h>
#include "perlin.h"
#include "watchface/view.h"
#include "watchface/background.h"
#include "service/resource_broker.h"


#if DEV_MODE
  #define NUM_TEXT_LAYERS 5
#else
  #define NUM_TEXT_LAYERS 4
#endif

/*********************** TextLayers ***********************/

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
    tlcs[0] = (TextLayerConfig) { TOP,    GRect(70, 5, 36, 18),   GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    tlcs[1] = (TextLayerConfig) { HOUR,   GRect(0, 16, 182, 64),  GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[2] = (TextLayerConfig) { MIN,    GRect(0, 73, 182, 64),  GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[3] = (TextLayerConfig) { BOTTOM, GRect(0, 141, 178, 18), GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    #if DEV_MODE
      tlcs[4] = (TextLayerConfig) { DEV,  GRect(20, 30, 30, 18),  GColorDarkCandyAppleRed, GColorWhite, get_date_font(), GTextAlignmentLeft };
    #endif
  #else // Pebble Time 
    tlcs[0] = (TextLayerConfig) { TOP,    GRect(50, 5, 36, 18),    GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    tlcs[1] =(TextLayerConfig)  { HOUR,   GRect(0, 15, 146, 64),   GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[2] =(TextLayerConfig)  { MIN,    GRect(0, 72, 146, 64),   GColorBlack, GColorClear, get_time_font(), GTextAlignmentCenter };
    tlcs[3] =(TextLayerConfig)  { BOTTOM, GRect(22, 145, 106, 18), GColorBlack, GColorWhite, get_date_font(), GTextAlignmentCenter };
    #if DEV_MODE
      tlcs[4] = (TextLayerConfig) { DEV,  GRect(0, 11, 30, 18),  GColorDarkCandyAppleRed, GColorWhite, get_date_font(), GTextAlignmentLeft };
    #endif
  #endif
  return tlcs;
}

void deinit_text_layer_configs(TextLayerConfig* tlcs){
  // Don't need to do a deep clean here-
  // Fonts need to stick around and are cleaned up elsewhere
  // Everything else isn't a pointer
  free(tlcs);
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
  // We never reference the text_layer_configs again; they're just for bootstrap
  deinit_text_layer_configs(text_layer_configs);  
  return wfv;
}

void deinit_text_layers(WatchfaceView* wfv){
  for(int i = 0; i < NUM_TEXT_LAYERS; i++) {
    text_layer_destroy(wfv->text_layers[i]);
  }  
}



/*********************** Window ***********************/




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

/*********************** View ***********************/


void attach_layers(WatchfaceView * wfv){
  layer_add_child( wfv->window_layer, 
                   bitmap_layer_get_layer( wfv->background->layer ) );
  for(int i = 0; i < NUM_TEXT_LAYERS; i++){
    layer_add_child(wfv->window_layer, text_layer_get_layer(wfv->text_layers[i]) );
  }
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

WatchfaceView* random_background(WatchfaceView* wfv) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Swapping Background");
  set_random_background(wfv->background);
  return wfv;
}

WatchfaceView* date_text_show(WatchfaceView* wfv){
  APP_LOG(APP_LOG_LEVEL_INFO, "Showing Date");
  layer_set_hidden(text_layer_get_layer(wfv->text_layers[BOTTOM]), false);
  return wfv;
}

WatchfaceView* date_text_hide(WatchfaceView* wfv){
  APP_LOG(APP_LOG_LEVEL_INFO, "Hiding Date"); 
  layer_set_hidden(text_layer_get_layer(wfv->text_layers[BOTTOM]), true);
  return wfv;
}

WatchfaceView* battery_text_show(WatchfaceView* wfv){
  APP_LOG(APP_LOG_LEVEL_INFO, "Showing Battery"); 
  layer_set_hidden(text_layer_get_layer(wfv->text_layers[TOP]), false);
  return wfv;
}

WatchfaceView* battery_text_hide(WatchfaceView* wfv){
  APP_LOG(APP_LOG_LEVEL_INFO, "Hiding Battery"); 
  layer_set_hidden(text_layer_get_layer(wfv->text_layers[TOP]), true);
  return wfv;
}

WatchfaceView* view_battery_text_update(WatchfaceView* wfv, int pct, bool is_charging){
  static char battery_text[] = "x100";

  if (is_charging) {
	snprintf(battery_text, sizeof(battery_text), "+%d", pct);
  } else {
	snprintf(battery_text, sizeof(battery_text), "%d", pct);
  }
  text_layer_set_text(wfv->text_layers[TOP], battery_text);
  
  return wfv;
}


