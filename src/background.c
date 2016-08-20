#include <pebble.h>
#include "background.h"

GBitmap * background_image;
BitmapLayer * background_layer;

#define RESOURCE_COUNT 19

static RESOURCE_ID background_image_resource_ids[RESOURCE_COUNT] = {
  RESOURCE_ID_IMAGE_BG1,
  RESOURCE_ID_IMAGE_BG2,
  RESOURCE_ID_IMAGE_BG3,
  RESOURCE_ID_IMAGE_BG4,
  RESOURCE_ID_IMAGE_BG5,
  RESOURCE_ID_IMAGE_BG6,
  RESOURCE_ID_IMAGE_BG7,
  RESOURCE_ID_IMAGE_BG8,
  RESOURCE_ID_IMAGE_BG9,
  RESOURCE_ID_IMAGE_BG10,
  RESOURCE_ID_IMAGE_BG11,
  RESOURCE_ID_IMAGE_BG12,
  RESOURCE_ID_IMAGE_BG13,
  RESOURCE_ID_IMAGE_BG14,
  RESOURCE_ID_IMAGE_BG15,
  RESOURCE_ID_IMAGE_BG16,
  RESOURCE_ID_IMAGE_BG17,
  RESOURCE_ID_IMAGE_BG18,
  RESOURCE_ID_IMAGE_BG19,
};


RESOURCE_ID choose_background_image_resource_id( int key ) {
  // saftey first!
  return background_image_resource_ids[key%RESOURCE_COUNT]; 
}

GBitmap * select_new_background_image( ) {
  int random_selection;
  RESOURCE_ID r_id;
  GBitmap * new_bg_image;

  random_selection = rand( );
  r_id = choose_background_image_resource_id(random_selection);
  new_bg_image = gbitmap_create_with_resource(r_id);
  return new_bg_image;    
}

void theme_choice( ) {	
    
  // collect previous image
  if (background_image) {
    gbitmap_destroy(background_image);
  	background_image = NULL;
  }
  // set new image
  background_image = select_new_background_image();
      
  if (background_image != NULL) {
		bitmap_layer_set_bitmap(background_layer, background_image);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
  }
  // TODO log warning here if Null is selected
}

void init_background( Layer * window_layer ){
  background_image = select_new_background_image( );
  background_layer = bitmap_layer_create( layer_get_frame( window_layer ) );
  bitmap_layer_set_bitmap( background_layer, background_image );

}