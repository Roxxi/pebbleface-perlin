#include <pebble.h>
#include "watchface/background.h"
#include "service/resource_broker.h"




WatchfaceBackground* init_background( Layer* parent_layer ){
  
  WatchfaceBackground* wfb = malloc(sizeof(WatchfaceBackground));
  wfb->image = select_random_background_image();
  wfb->layer = bitmap_layer_create( layer_get_frame( parent_layer ) );
  bitmap_layer_set_bitmap( wfb->layer, wfb->image );
  return wfb; 
}

bool is_wfb_image_set(WatchfaceBackground* wfb){
  return wfb->image != NULL;
}

void dealloc_wfb_image(WatchfaceBackground* wfb){
  if (is_wfb_image_set(wfb)) {
    gbitmap_destroy(wfb->image);
  	wfb->image = NULL;
  }
}

void dealloc_wfb_layer(WatchfaceBackground* wfb) {
  layer_remove_from_parent(bitmap_layer_get_layer(wfb->layer));
  bitmap_layer_destroy(wfb->layer);
}


void deinit_background(WatchfaceBackground* wfb){
  dealloc_wfb_layer(wfb);
  dealloc_wfb_image(wfb);
  free(wfb);
}



void set_random_background(WatchfaceBackground* wfb) {	
    
  // clean up previous image - first to free space for the read
  dealloc_wfb_image(wfb);
  // select new image
  wfb->image = select_random_background_image();
  // set new background
  bitmap_layer_set_bitmap(wfb->layer, wfb->image);
  // redraw the screen
	layer_set_hidden(bitmap_layer_get_layer(wfb->layer), false);
	layer_mark_dirty(bitmap_layer_get_layer(wfb->layer));
}



