#pragma once

#include <pebble.h>

typedef uint32_t RESOURCE_ID;

extern GBitmap * background_image;
extern BitmapLayer * background_layer;

void theme_choice();
void init_background( Layer * window_layer );