#pragma once

#include <pebble.h>

typedef struct {
  BitmapLayer * layer;
  GBitmap * image;
} WatchfaceBackground;

void set_random_background(WatchfaceBackground*);
WatchfaceBackground* init_background(Layer*);
void deinit_background(WatchfaceBackground*);