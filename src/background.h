#pragma once

#include <pebble.h>

typedef uint32_t RESOURCE_ID;



void set_random_background();
void init_background( Layer * window_layer );
void deinit_background( );