#include <pebble.h>
#include "perlin.h"
#include "watchface/app.h"

static WatchfaceApp* app;

void handle_init(void) {
  app = init_watchface_app();
  watchface_app_update(app);
}
 

void handle_deinit(void) {
  deinit_watchface_app(app);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
