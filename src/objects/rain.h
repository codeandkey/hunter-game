#pragma once

#include <tds/object.h>

#define HUNTER_RAIN_LIGHTNING_MS_MIN 5000
#define HUNTER_RAIN_LIGHTNING_MS_VAR 5000
#define HUNTER_RAIN_LIGHTNING_INT_MIN 0.2f
#define HUNTER_RAIN_LIGHTNING_INT_VAR 0.1f

#define HUNTER_RAIN_LIGHTNING_PRECISION 100
#define HUNTER_RAIN_LIGHTNING_DECAY 1.1f

struct tds_object_type obj_rain_type;

void obj_rain_init(struct tds_object* ptr);
void obj_rain_destroy(struct tds_object* ptr);
void obj_rain_update(struct tds_object* ptr);
void obj_rain_draw(struct tds_object* ptr);
void obj_rain_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_rain_data {
	tds_clock_point start_point;
	int ms_interval;
	float lightning;
	int unused;
};
