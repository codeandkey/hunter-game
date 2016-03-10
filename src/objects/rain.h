#pragma once

#include <tds/object.h>

struct tds_object_type obj_rain_type;

void obj_rain_init(struct tds_object* ptr);
void obj_rain_destroy(struct tds_object* ptr);
void obj_rain_update(struct tds_object* ptr);
void obj_rain_draw(struct tds_object* ptr);
void obj_rain_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_rain_data {
	int unused;
};
