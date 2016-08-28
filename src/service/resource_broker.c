#include <pebble.h>
#include "resource_broker.h"

typedef uint32_t ResourceId;


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

static const ResourceId TIME_FONT_RID = RESOURCE_ID_FONT_LECO_62;
static const ResourceId DATE_FONT_RID = RESOURCE_ID_FONT_LECO_14;


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


GFont load_font(ResourceId resource_id){
  return fonts_load_custom_font(resource_get_handle(resource_id));
}

static GFont memo_time_font = NULL;
GFont get_time_font(){
  if (memo_time_font == NULL){
    memo_time_font = load_font(TIME_FONT_RID);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded Time Font");
  }
  return memo_time_font;
}

static GFont memo_date_font = NULL;
GFont get_date_font(){
  if (memo_date_font == NULL){
    memo_date_font = load_font(DATE_FONT_RID);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded Date Font");
  }
  return memo_date_font;
}

void deinit_fonts(){
  fonts_unload_custom_font(memo_time_font);
  fonts_unload_custom_font(memo_date_font);
}