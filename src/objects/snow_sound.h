#pragma once

#include <tds/object.h>

extern struct tds_object_type obj_snow_sound_type;

void obj_snow_sound_update(struct tds_object* ptr);
void obj_snow_sound_init(struct tds_object* ptr);
void obj_snow_sound_destroy(struct tds_object* ptr);
