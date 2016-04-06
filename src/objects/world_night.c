#include "world_night.h"
#include "camera.h"

#include <tds/tds.h>

#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_world_night_type = {
	.type_name = "obj_world_night",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_world_night_data),
	.func_init = obj_world_night_init,
	.func_destroy = obj_world_night_destroy,
	.func_update = obj_world_night_update,
	.func_draw = obj_world_night_draw,
	.func_msg = obj_world_night_msg,
	.save = 1,
};

void obj_world_night_init(struct tds_object* ptr) {
	struct obj_world_night_data* data = (struct obj_world_night_data*) ptr->object_data;

	/* The purpose of this object is to do everything necessary to set up the "night" environment.
	 * We add some backgrounds and set the ambient light level. */

	tds_bg_push(tds_engine_global->bg_handle, 0, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_night_colored_640x640.png", 640, 640, 1, 1), 0, 0);
	tds_bg_push(tds_engine_global->bg_handle, 1, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_night_stars_640x640.png", 640, 640, 1, 1), 1, 0);

	tds_render_set_ambient_brightness(tds_engine_global->render_handle, 0.1f);
}

void obj_world_night_destroy(struct tds_object* ptr) {
}

void obj_world_night_update(struct tds_object* ptr) {
}

void obj_world_night_draw(struct tds_object* ptr) {
}

void obj_world_night_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
