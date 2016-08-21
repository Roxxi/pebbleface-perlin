#pragma once

#include <pebble.h>

typedef uint32_t ResourceId;

typedef struct {
  BitmapLayer * layer;
  GBitmap * image;
} WatchFaceBackground;

void set_random_background();
void init_background_layer( Layer * window_layer );
void deinit_background_layer( );