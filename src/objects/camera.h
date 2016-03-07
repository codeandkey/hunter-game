#pragma once
#include <tds/object.h>

#define TDS_OBJ_CAMERA_POS_FACTOR 4.0f
#define TDS_OBJ_CAMERA_SIZE_FACTOR 2.0f

#define TDS_OBJ_CAMERA_SIZE_DIV 4.0f

struct tds_object_type obj_camera_type;

void obj_camera_init(struct tds_object* ptr);
void obj_camera_destroy(struct tds_object* ptr);
void obj_camera_update(struct tds_object* ptr);
void obj_camera_draw(struct tds_object* ptr);
void obj_camera_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_camera_data {
	struct tds_object* track;
	float shake, size, target_size;
};
