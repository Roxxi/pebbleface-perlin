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
