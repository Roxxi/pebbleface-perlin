#include <pebble.h>
#include "background.h"
#include "resource_broker.h"

typedef struct {
  BitmapLayer * layer;
  GBitmap * image;
} WatchFaceBackground;


/*********** WatchFaceBackground ***********/
bool is_wfb_image_set(WatchFaceBackground* wfb){
  return wfb->image != NULL;
}

void dealloc_wfb_image(WatchFaceBackground* wfb){
  if (is_wfb_image_set(wfb)) {
    gbitmap_destroy(wfb->image);
  	wfb->image = NULL;
  }
}

void update_wfb_image(WatchFaceBackground* wfb, GBitmap* bg_image){
  dealloc_wfb_image(wfb);
  wfb->image = bg_image;
}

void dealloc_wfb_layer(WatchFaceBackground* wfb) {
  layer_remove_from_parent(bitmap_layer_get_layer(wfb->layer));
  bitmap_layer_destroy(wfb->layer);
}

// No need for `update_wfb_layer`


/************ BG Layer Singleton **************/
static WatchFaceBackground* theWatchFaceBackground;

WatchFaceBackground* get_watchfacebackground(){
  return theWatchFaceBackground;
}

WatchFaceBackground* set_watchfacebackground(WatchFaceBackground* wfb){
  theWatchFaceBackground = wfb;
  return theWatchFaceBackground;
}

void update_wfb_background(WatchFaceBackground* wfb, GBitmap * bg_image){
  if (bg_image != NULL) {
    update_wfb_image(wfb, bg_image);
    bitmap_layer_set_bitmap(wfb->layer, wfb->image);
		layer_set_hidden(bitmap_layer_get_layer(wfb->layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(wfb->layer));
  }
}

void dealloc_the_background_layer(){
  dealloc_wfb_layer(theWatchFaceBackground);
  dealloc_wfb_image(theWatchFaceBackground);
  free(theWatchFaceBackground);
}


/************ Public Methods **************/

void set_random_background() {	
    
  GBitmap* bg_image;
  WatchFaceBackground* wfb;

  wfb = get_watchfacebackground();
  // clean up previous image - first to free space for the read
  dealloc_wfb_image(wfb);
  // select new image
  bg_image = select_random_background_image();
  // redraw with new background
  update_wfb_background(wfb, bg_image);
}

void init_background_layer( Layer * parent_layer ){
  
  WatchFaceBackground* wfb = malloc(sizeof(WatchFaceBackground));
  set_watchfacebackground(wfb);
  wfb->image = select_random_background_image();
  wfb->layer = bitmap_layer_create( layer_get_frame( parent_layer ) );
  bitmap_layer_set_bitmap( wfb->layer, wfb->image );
  layer_add_child( parent_layer, bitmap_layer_get_layer( wfb->layer ) );
}

void deinit_background_layer(){
  dealloc_the_background_layer();
}



