/*
  Copyright (C) 2015 Mark Reed

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <pebble.h>
#include "perlin2.h"
#include "background.h"
#include "resource_broker.h"
#include "watchface_view.h"

// set to 0 to for production builds



/**** Comms ****/

static AppSync sync;
static uint8_t sync_buffer[256];

/**** Bootstrapping flow control ****/
static bool appStarted = false;


int cur_day = -1;

int charge_percent = 0;

/**** Settings ****/

static int showdate;
static int bluetoothvibe;
static int hourlyvibe;
static int showbatt;
static int randomtime;

enum {
  DATE_KEY = 0x0,
  BLUETOOTHVIBE_KEY = 0x1,
  HOURLYVIBE_KEY = 0x2,
  BATT_KEY = 0x3,
  RANDOMTIME_KEY = 0x4
};

/**** UI ****/

TextLayer *layer_date_text;
TextLayer *layer_time_hour_text;
TextLayer *layer_time_min_text;
TextLayer *battery_text_layer;

static WatchfaceView* watchface_view;
void init_view(){
  watchface_view = init_watchface_view();
  window_stack_push(watchface_view->window, true); // make the window appear on top
  layer_date_text = watchface_view->text_layers[BOTTOM];
  layer_time_hour_text = watchface_view->text_layers[HOUR];
  layer_time_min_text = watchface_view->text_layers[MIN];
  battery_text_layer = watchface_view->text_layers[TOP];
}
void deinit_view(){
  deinit_watchface_view(watchface_view);
}









static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {

  case DATE_KEY:
	showdate = new_tuple->value->uint8 != 0;
	persist_write_bool(DATE_KEY, showdate);
	if (showdate) {
	  layer_set_hidden(text_layer_get_layer(layer_date_text), true);
	} else {
	  layer_set_hidden(text_layer_get_layer(layer_date_text), false);
	}
	break;

  case BLUETOOTHVIBE_KEY:
	bluetoothvibe = new_tuple->value->uint8 != 0;
	persist_write_bool(BLUETOOTHVIBE_KEY, bluetoothvibe);
	break;

  case HOURLYVIBE_KEY:
	hourlyvibe = new_tuple->value->uint8 != 0;
	persist_write_bool(HOURLYVIBE_KEY, hourlyvibe);
	break;

  case BATT_KEY:
	showbatt = new_tuple->value->uint8  !=0;
	persist_write_bool(BATT_KEY, showbatt);
	if (showbatt) {
	  layer_set_hidden(text_layer_get_layer(battery_text_layer), true);
	} else {
	  layer_set_hidden(text_layer_get_layer(battery_text_layer), false);
	}
	break;

  case RANDOMTIME_KEY:
	randomtime = new_tuple->value->uint8 != 0;
	persist_write_bool(RANDOMTIME_KEY, randomtime);
	if (randomtime) {
	  set_random_background();
	}
	break;
  } // end switch
} // end function

void update_battery_state(BatteryChargeState charge_state) {
  static char battery_text[] = "x100";

  if (charge_state.is_charging) {
	snprintf(battery_text, sizeof(battery_text), "+%d", charge_state.charge_percent);
  } else {
	snprintf(battery_text, sizeof(battery_text), "%d", charge_state.charge_percent);
  }
  charge_percent = charge_state.charge_percent;
  text_layer_set_text(battery_text_layer, battery_text);
}

static void toggle_bluetooth(bool connected) {

  if (appStarted && !connected && bluetoothvibe) {
    vibes_long_pulse(); //vibe!
  }
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth(connected);
}

void update_time(struct tm *tick_time) {

  static char h_time_text[] = "00";
  static char m_time_text[] = "00";
  static char date_text[] = "XXX XXX 00XX XXX";

  char *h_time_format;
  int new_cur_day = tick_time->tm_year*1000 + tick_time->tm_yday;

  if (new_cur_day != cur_day) {
    cur_day = new_cur_day;

	switch(tick_time->tm_mday) {
	case 1 :
	case 21 :
	case 31 :
	  strftime(date_text, sizeof(date_text), "%a, %est %b", tick_time);
	  break;
	case 2 :
	case 22 :
      strftime(date_text, sizeof(date_text), "%a, %end %b", tick_time);
      break;
    case 3 :
    case 23 :
      strftime(date_text, sizeof(date_text), "%a, %erd %b", tick_time);
      break;
    default :
      strftime(date_text, sizeof(date_text), "%a, %eth %b", tick_time);
      break;
    } // end switch
    text_layer_set_text(layer_date_text, date_text);
  } // end if
  if (clock_is_24h_style()) {
    h_time_format = "%H";
  } else {
    h_time_format = "%I";
  }
  strftime(h_time_text, sizeof(h_time_text), h_time_format, tick_time);
  text_layer_set_text(layer_time_hour_text, h_time_text);

  strftime(m_time_text, sizeof(m_time_text), "%M", tick_time);
  text_layer_set_text(layer_time_min_text, m_time_text);
}

void hourvibe (struct tm *tick_time) {

  if(appStarted && hourlyvibe) {
	//vibe!
	vibes_short_pulse();
  }
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);

  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Minute Updates");

  if (units_changed & HOUR_UNIT) {
    hourvibe(tick_time);
    set_random_background();
  }
#if DEV_MODE == 1
  if (units_changed & MINUTE_UNIT) {
    APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Updating Background on the Minute");
    set_random_background();  // used for testing
  }

#endif

}

void force_update(void) {
  toggle_bluetooth(bluetooth_connection_service_peek());
  time_t now = time(NULL);
  update_time(localtime(&now));
}



void handle_init(void) {

  const int inbound_size = 256;
  const int outbound_size = 256;
  app_message_open(inbound_size, outbound_size);

  init_view();



  // handlers
  battery_state_service_subscribe(&update_battery_state);
  bluetooth_connection_service_subscribe(&toggle_bluetooth);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);

  Tuplet initial_values[] = {
	TupletInteger(DATE_KEY, persist_read_bool(DATE_KEY)),
    TupletInteger(BLUETOOTHVIBE_KEY, persist_read_bool(BLUETOOTHVIBE_KEY)),
    TupletInteger(HOURLYVIBE_KEY, persist_read_bool(HOURLYVIBE_KEY)),
    TupletInteger(BATT_KEY, persist_read_bool(BATT_KEY)),
    TupletInteger(RANDOMTIME_KEY, persist_read_bool(RANDOMTIME_KEY)),
  };

  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
				sync_tuple_changed_callback, NULL, NULL);

  appStarted = true;

  // update the battery on launch
  update_battery_state(battery_state_service_peek());

  // draw first frame
  force_update();
}

void handle_deinit(void) {
  app_sync_deinit(&sync);

  tick_timer_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  battery_state_service_unsubscribe();

  

}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
