#include <pebble.h>
#include "perlin.h"
#include "watchface/state.h"
#include "settings.h"

/*** Macro Notes
 * 
 * This shit is cray-cray. Unfortunately you can't use ## concatenation operator
 * to build a string.
 * [Here's what I was trying to do](https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html#Concatenation)
 * 
 * (This is the error I was encoutering: `pasting "->" and "data" does not give a valid preprocessing token`)
 * But in the end, I couldn't concatenate `state->ATTRIBUTE` because there are
 * three tokens: `state`, `->`, and `ATTRIBUTE` and neither `state->` nor `->ATTRIBUTE`
 * are valid identifiers on their own- and each concatenation must yield valid i
 * identifiers.
 *
 * This article, at the very bottom mentions a trick by using `blockquotes (/ ** /)` to eat
 * merge the values, by having the comments eaten out by the pre-processor
 * * [Stackoverflow that helped](https://stackoverflow.com/questions/13715424/token-pasting-in-c-with-structure-dereference-operator/19457240#19457240?newreg=768b3027da214b82bbd33935c29e1221)
 * * [Original Article, see historical note](http://www.complete-concrete-concise.com/programming/c/preprocessor-the-token-pasting-operator)
 *
 */

#define state_read_type_property(TYPE, ATTRIBUTE)     \
TYPE state_read_##ATTRIBUTE(WatchfaceState* state){   \
  return state->/**/ATTRIBUTE;                        \
}

state_read_type_property(bool, bluetooth_vibe);
state_read_type_property(bool, hourly_vibe);
state_read_type_property(bool, random_background);
state_read_type_property(bool, show_date);
state_read_type_property(bool, show_battery);
state_read_type_property(int, current_day);
state_read_type_property(int, charge_percent);

#define state_update_type_property(TYPE, ATTRIBUTE)                   \
void state_update_##ATTRIBUTE(WatchfaceState* state, TYPE ATTRIBUTE){ \
   state->/**/ATTRIBUTE = ATTRIBUTE;                                  \
}

state_update_type_property(bool, bluetooth_vibe);
state_update_type_property(bool, hourly_vibe);
state_update_type_property(bool, random_background);
state_update_type_property(bool, show_date);
state_update_type_property(bool, show_battery);
state_update_type_property(int, current_day);
state_update_type_property(int, charge_percent);


/*** Persistent Storage Mangement ***/

// Persistent Storage Keys
enum {
  show_date_KEY = 0x0,
  bluetooth_vibe_KEY = 0x1,
  hourly_vibe_KEY = 0x2,
  show_battery_KEY = 0x3,
  random_background_KEY = 0x4
};

#define persist_read_property(PROPERTY)                     \
  state->/**/PROPERTY = persist_read_bool(PROPERTY##_KEY);
void state_load_settings(WatchfaceState* state){
  persist_read_property(show_date);
  persist_read_property(bluetooth_vibe);
  persist_read_property(hourly_vibe);
  persist_read_property(show_battery);
  persist_read_property(random_background);
}

#define persist_write_property(PROPERTY)                    \
  persist_write_bool(PROPERTY##_KEY, state->/**/PROPERTY);
void state_persist_settings(WatchfaceState* state){
  persist_write_property(show_date);
  persist_write_property(bluetooth_vibe);
  persist_write_property(hourly_vibe);
  persist_write_property(show_battery);
  persist_write_property(random_background);
}

WatchfaceState* init_watchface_state(){
  WatchfaceState* state = malloc(sizeof(WatchfaceState));

  state_load_settings(state);
  state_update_current_day(state, -1);
  state_update_charge_percent(state, 0);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, 
          "{ 'bluetoothview':%d, 'hourlyvibe':%d, 'randomwallpaper':%d, 'showdate':%d, 'showbatt':%d, 'cur_day':%d, 'charge_percent': %d }",
          state_read_bluetooth_vibe(state),
          state_read_hourly_vibe(state), 
          state_read_random_background(state),
          state_read_show_date(state),
          state_read_show_battery(state),
          state_read_current_day(state),
          state_read_charge_percent(state));
  return state;
}


void deinit_watchface_state(WatchfaceState* state){
  free(state);
}

