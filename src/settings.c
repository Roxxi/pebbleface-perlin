#include <pebble.h>
#include "perlin.h"
#include "settings.h"
#include "watchface_view.h"


enum {
  DATE_KEY = 0x0,
  BLUETOOTHVIBE_KEY = 0x1,
  HOURLYVIBE_KEY = 0x2,
  BATT_KEY = 0x3,
  RANDOMTIME_KEY = 0x4
};

void handle_setting_showdate(Tuple* setting, void* context){
  bool showdate;
  showdate = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "showdate: %d", showdate);
  persist_write_bool(DATE_KEY, showdate);
  showdate ? 
    date_text_show(watchface_view): 
    date_text_hide(watchface_view);
}

void handle_setting_showbatt(Tuple* setting, void* context){
  bool showbatt;
  showbatt = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "showbatt: %d", showbatt);
  persist_write_bool(BATT_KEY, showbatt);
  showbatt ? 
    battery_text_show(watchface_view): 
    battery_text_hide(watchface_view);
}

void handle_setting_randomwallpaper(Tuple* setting, void* context){
  bool randomwallpaper;
  randomwallpaper = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "randomwallpaper: %d", randomwallpaper);
  persist_write_bool(RANDOMTIME_KEY, randomwallpaper);
  if (randomwallpaper) {
    random_background(watchface_view);
  }
}

void handle_setting_bluetoothvibe(Tuple* setting, void* context){
  bool bluetoothvibe;
  bluetoothvibe = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "bluetoothvibe: %d", bluetoothvibe);
  persist_write_bool(BLUETOOTHVIBE_KEY, bluetoothvibe);
}

void handle_setting_hourlyvibe(Tuple* setting, void* context){
  bool hourlyvibe;
  hourlyvibe = setting->value->uint8 == 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "hourlyvibe: %d", hourlyvibe);
  persist_write_bool(HOURLYVIBE_KEY, hourlyvibe);
}

void handle_setting(Tuple* setting, void* context){
  const uint32_t key = setting->key;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting Key: %d", (int) key);
  if (key == MESSAGE_KEY_showdate) {
      handle_setting_showdate(setting, context);
  } else if (key == MESSAGE_KEY_showbatt) {
      handle_setting_showbatt(setting, context);
  } else if (key == MESSAGE_KEY_randomwallpaper) {
      handle_setting_randomwallpaper(setting, context);
  } else if (key == MESSAGE_KEY_bluetoothvibe) {
      handle_setting_bluetoothvibe(setting, context);
  } else if (key == MESSAGE_KEY_hourlyvibe) {
      handle_setting_hourlyvibe(setting, context);
  }
}

void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *cur;
  for(cur = dict_read_first(iter); cur != NULL ; cur = dict_read_next(iter)) {
    handle_setting(cur, context);
  }
}


void init_settings(){
/*

  
  const int inbound_size = 256; // should be dyanmically determined by tuplet size
  const int outbound_size = 256;

  APP_LOG(APP_LOG_LEVEL_INFO, "Default date: %d", persist_read_bool(DATE_KEY));
  APP_LOG(APP_LOG_LEVEL_INFO, "Default batt: %d", persist_read_bool(BATT_KEY));
  Tuplet initial_values[] = {
	  TupletInteger(DATE_KEY, persist_read_bool(DATE_KEY)),
    TupletInteger(BLUETOOTHVIBE_KEY, persist_read_bool(BLUETOOTHVIBE_KEY)),
    TupletInteger(HOURLYVIBE_KEY, persist_read_bool(HOURLYVIBE_KEY)),
    TupletInteger(BATT_KEY, persist_read_bool(BATT_KEY)),
    TupletInteger(RANDOMTIME_KEY, persist_read_bool(RANDOMTIME_KEY)),
  };


  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
				sync_tuple_changed_callback, NULL, NULL);
  app_sync_init(&sync, sync_buffer, 256, const Tuplet *const keys_and_initial_values, const uint8_t count, AppSyncTupleChangedCallback tuple_changed_callback, AppSyncErrorCallback error_callback, void *context)
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(inbound_size, outbound_size); 
*/
}
 