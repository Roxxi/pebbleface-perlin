#include <pebble.h>
#include "perlin.h"
#include "watchface/app.h"

static WatchfaceApp* app;

void handle_init(void) {
  app = watchface_app_init();
  watchface_app_bootstrap(app);
}
 

void handle_deinit(void) {
  watchface_app_deinit(app);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
