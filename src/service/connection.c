#include <pebble.h>
#include "service/connection.h"
#include "watchface/app.h"



static void app_connection_handler(bool connected) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Pebble app %sconnected", connected ? "" : "dis");
  app_update_bluetooth_connection(watchface_app(), connected);
}

static void kit_connection_handler(bool connected) {
  // Always false on Android
  APP_LOG(APP_LOG_LEVEL_INFO, "PebbleKit %sconnected", connected ? "" : "dis");
  app_update_bluetooth_connection(watchface_app(), connected); 
}

void service_connection_service_subscribe() {
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = app_connection_handler,
    .pebblekit_connection_handler = kit_connection_handler
  });
}

void service_connection_service_unsubscribe() {
  connection_service_unsubscribe();
}

void service_connection_service_update() {
  bool app_connection = connection_service_peek_pebble_app_connection();
  bool kit_connection = connection_service_peek_pebblekit_connection();

  if (app_connection) {
    app_connection_handler(app_connection);
  } else {
    kit_connection_handler(kit_connection);
  }
}