#pragma once

#include <pebble.h>


void prv_inbox_received_handler(DictionaryIterator *iter, void *context);

bool setting_read_bluetooth();
bool setting_read_hourlyvibe();
bool setting_read_randomwallpaper();
bool setting_read_showdate();
bool setting_read_showbatt();
  