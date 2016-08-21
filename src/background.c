#include <pebble.h>
#include "background.h"



#define RESOURCE_COUNT 19

static ResourceId background_image_resource_ids[RESOURCE_COUNT] = {
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


ResourceId choose_background_image_resource_id( int key ) {

  return background_image_resource_ids[key % RESOURCE_COUNT]; 
}

GBitmap * select_random_background_image() {
  int random_selection;
  ResourceId r_id;
  GBitmap * new_bg_image;

  random_selection = rand( );
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Random Val: %d", random_selection);
  r_id = choose_background_image_resource_id(random_selection);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Choosing Background Image resource %d", (int) r_id);

  new_bg_image = gbitmap_create_with_resource(r_id);
  return new_bg_image;    
}

bool is_background_image_set(BackgroundLayer* bg){
  return bg->image != NULL;
}

void dealloc_background_image(BackgroundLayer* bg){
  if (is_background_image_set(bg)) {
    gbitmap_destroy(bg->image);
  	bg->image = NULL;
  }
}

void update_background_image(BackgroundLayer* bg, GBitmap* bg_image){
  dealloc_background_image(bg);
  bg->image = bg_image;
}



void dealloc_background_layer(BackgroundLayer* bg) {
  layer_remove_from_parent(bitmap_layer_get_layer(bg->layer));
  bitmap_layer_destroy(bg->layer);
}

void update_layer_background(BackgroundLayer* bg, GBitmap * bg_image){
  if (bg_image != NULL) {
    update_background_image(bg, bg_image);
    bitmap_layer_set_bitmap(bg->layer, bg->image);
		layer_set_hidden(bitmap_layer_get_layer(bg->layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(bg->layer));
  }
}


/************ BG Layer Singleton **************/
static BackgroundLayer* theBackgroundLayer;

BackgroundLayer* get_background_layer(){
  return theBackgroundLayer;
}

BackgroundLayer* set_background_layer(BackgroundLayer* bg){
  theBackgroundLayer = bg;
  return theBackgroundLayer;
}

void dealloc_the_background_layer(){
  dealloc_background_layer(theBackgroundLayer);
  dealloc_background_image(theBackgroundLayer);
  free(theBackgroundLayer);
}


/************ Public Methods **************/

void set_random_background() {	
    
  GBitmap* bg_image;
  BackgroundLayer* bg = get_background_layer();

  // clean up previous image
  dealloc_background_image(bg);

  // select new image
  bg_image = select_random_background_image();
  update_layer_background(bg, bg_image);
}

void init_background_layer( Layer * parent_layer ){
  
  BackgroundLayer* bg = malloc(sizeof(BackgroundLayer));
  set_background_layer(bg);
  bg->image = select_random_background_image();
  bg->layer = bitmap_layer_create( layer_get_frame( parent_layer ) );
  bitmap_layer_set_bitmap( bg->layer, bg->image );
  layer_add_child( parent_layer, bitmap_layer_get_layer( bg->layer ) );
}

void deinit_background_layer(){
  dealloc_the_background_layer();
}



