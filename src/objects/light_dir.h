#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_LIGHT_DIR_INDEX_DX 0
#define HUNTER_LIGHT_DIR_INDEX_DY 1
#define HUNTER_LIGHT_DIR_INDEX_R 2
#define HUNTER_LIGHT_DIR_INDEX_G 3
#define HUNTER_LIGHT_DIR_INDEX_B 4
#define HUNTER_LIGHT_DIR_INDEX_FLICKER 5

struct tds_object_type obj_light_dir_type;

void obj_light_dir_init(struct tds_object* ptr);
void obj_light_dir_destroy(struct tds_object* ptr);
void obj_light_dir_update(struct tds_object* ptr);
void obj_light_dir_draw(struct tds_object* ptr);
void obj_light_dir_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_light_dir_data {
	float dir_x, dir_y;
	float r, g, b;
	float flicker;
};
