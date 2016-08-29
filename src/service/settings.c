#include <pebble.h>

#include "service/settings.h"
#include "watchface/app.h"
#include "watchface/view.h"
#include "watchface/state.h"

//TODO create app interface to remove state calls

void handle_setting_showdate(Tuple* setting, WatchfaceApp* app){
  bool showdate;
  showdate = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "showdate: %d", showdate);
  state_update_show_date(app->state, showdate);
}

void handle_setting_showbatt(Tuple* setting, WatchfaceApp* app){
  bool showbatt;
  showbatt = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "showbatt: %d", showbatt);
  state_update_show_battery(app->state, showbatt);
}

void handle_setting_random_background(Tuple* setting, WatchfaceApp* app){
  bool random_background;
  random_background = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "random_background: %d", random_background);
  state_update_random_background(app->state, random_background);
}

void handle_setting_bluetooth_vibe(Tuple* setting, WatchfaceApp* app){
  bool bluetoothvibe;
  bluetoothvibe = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "bluetoothvibe: %d", bluetoothvibe);
  state_update_bluetooth_vibe(app->state, bluetoothvibe);
}

void handle_setting_hourly_vibe(Tuple* setting, WatchfaceApp* app){
  bool hourlyvibe;  
  hourlyvibe = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "hourlyvibe: %d", hourlyvibe);
  state_update_hourly_vibe(app->state, hourlyvibe);

}

void handle_setting(Tuple* setting, WatchfaceApp* app){
  const uint32_t key = setting->key;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting Key: %d", (int) key);
  if (key == MESSAGE_KEY_showdate) {
      handle_setting_showdate(setting, app);
  } else if (key == MESSAGE_KEY_showbatt) {
      handle_setting_showbatt(setting, app);
  } else if (key == MESSAGE_KEY_randomwallpaper) {
      handle_setting_random_background(setting, app);
  } else if (key == MESSAGE_KEY_bluetoothvibe) {
      handle_setting_bluetooth_vibe(setting, app);
  } else if (key == MESSAGE_KEY_hourlyvibe) {
      handle_setting_hourly_vibe(setting, app);
  }
}

void settings_inbox_received_handler(DictionaryIterator *iter, void *context) {

  WatchfaceApp* app;
  Tuple *cur;

  app = (WatchfaceApp*) context;

  for(cur = dict_read_first(iter); cur != NULL ; cur = dict_read_next(iter)) {
    handle_setting(cur, context);
  }
  app_handle_apply_settings(app);

}








 