#pragma once

#include <tds/object.h>

#define HUNTER_BGM_VOLUME_SLOPE 0.006f

#define HUNTER_BGM_INDEX_SOUND 0
#define HUNTER_BGM_INDEX_DONTSTART 1
#define HUNTER_BGM_INDEX_CONTROL_INDEX 2

struct tds_object_type obj_bgm_type;

void obj_bgm_init(struct tds_object* ptr);
void obj_bgm_destroy(struct tds_object* ptr);
void obj_bgm_update(struct tds_object* ptr);
void obj_bgm_draw(struct tds_object* ptr);
void obj_bgm_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_bgm_data {
	int should_play;
	float volume;
	int control_id;
};
