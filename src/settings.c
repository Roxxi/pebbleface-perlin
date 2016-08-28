#include <pebble.h>

#include "settings.h"
#include "watchface/app.h"
#include "watchface/view.h"


enum {
  DATE_KEY = 0x0,
  BLUETOOTHVIBE_KEY = 0x1,
  HOURLYVIBE_KEY = 0x2,
  BATT_KEY = 0x3,
  RANDOMTIME_KEY = 0x4
};

void handle_setting_showdate(Tuple* setting, WatchfaceApp* app){
  bool showdate;
  
  showdate = setting->value->uint8 == 1;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "showdate: %d", showdate);
  persist_write_bool(DATE_KEY, showdate);
  app->state->showdate = showdate;
  showdate ? 
    date_text_show(app->view): 
    date_text_hide(app->view);
}

void handle_setting_showbatt(Tuple* setting, WatchfaceApp* app){
  bool showbatt;
 
  showbatt = setting->value->uint8 == 1;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "showbatt: %d", showbatt);
  persist_write_bool(BATT_KEY, showbatt);
  app->state->showbatt = showbatt;
  showbatt ? 
    battery_text_show(app->view): 
    battery_text_hide(app->view);
}

void handle_setting_randomwallpaper(Tuple* setting, WatchfaceApp* app){
  bool randomwallpaper;
 
  randomwallpaper = setting->value->uint8 == 1;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "randomwallpaper: %d", randomwallpaper);
  persist_write_bool(RANDOMTIME_KEY, randomwallpaper);
  app->state->randomwallpaper = randomwallpaper;
  if (randomwallpaper) {
    random_background(app->view);
  }
}

void handle_setting_bluetoothvibe(Tuple* setting, WatchfaceApp* app){
  bool bluetoothvibe;

  bluetoothvibe = setting->value->uint8 == 1;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "bluetoothvibe: %d", bluetoothvibe);
  persist_write_bool(BLUETOOTHVIBE_KEY, bluetoothvibe);
  app->state->bluetoothvibe = bluetoothvibe;
}

void handle_setting_hourlyvibe(Tuple* setting, WatchfaceApp* app){
  bool hourlyvibe;
  
  hourlyvibe = setting->value->uint8 == 1;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "hourlyvibe: %d", hourlyvibe);
  persist_write_bool(HOURLYVIBE_KEY, hourlyvibe);
  app->state->hourlyvibe = hourlyvibe;
}

void handle_setting(Tuple* setting, void* context){
  WatchfaceApp* app;
  const uint32_t key = setting->key;
  app = (WatchfaceApp*) context;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting Key: %d", (int) key);
  if (key == MESSAGE_KEY_showdate) {
      handle_setting_showdate(setting, app);
  } else if (key == MESSAGE_KEY_showbatt) {
      handle_setting_showbatt(setting, app);
  } else if (key == MESSAGE_KEY_randomwallpaper) {
      handle_setting_randomwallpaper(setting, app);
  } else if (key == MESSAGE_KEY_bluetoothvibe) {
      handle_setting_bluetoothvibe(setting, app);
  } else if (key == MESSAGE_KEY_hourlyvibe) {
      handle_setting_hourlyvibe(setting, app);
  }
}

void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *cur;
  for(cur = dict_read_first(iter); cur != NULL ; cur = dict_read_next(iter)) {
    handle_setting(cur, context);
  }
}


bool setting_read_bluetooth() {
  return persist_read_bool(BLUETOOTHVIBE_KEY);
}
bool setting_read_hourlyvibe(){
  return persist_read_bool(HOURLYVIBE_KEY);
}
bool setting_read_randomwallpaper(){
  return persist_read_bool(RANDOMTIME_KEY);
}
bool setting_read_showdate(){
  return persist_read_bool(DATE_KEY);
}
bool setting_read_showbatt(){
  return persist_read_bool(BATT_KEY);
}



 