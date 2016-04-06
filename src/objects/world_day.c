#include "world_day.h"
#include "camera.h"

#include <tds/tds.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_world_day_type = {
	.type_name = "obj_world_day",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_world_day_data),
	.func_init = obj_world_day_init,
	.func_destroy = obj_world_day_destroy,
	.func_update = obj_world_day_update,
	.func_draw = obj_world_day_draw,
	.func_msg = obj_world_day_msg,
	.save = 1,
};

void obj_world_day_init(struct tds_object* ptr) {
	struct obj_world_day_data* data = (struct obj_world_day_data*) ptr->object_data;

	tds_bg_push(tds_engine_global->bg_handle, 0, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_overworld_sky_640x640.png", 640, 640, 1, 1), 0, 0);
	tds_bg_push(tds_engine_global->bg_handle, 3, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_largeisland_640x640.png", 640, 640, 1, 1), 1, 0);
	tds_bg_push(tds_engine_global->bg_handle, 5, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_ocean_640x640.png", 640, 640, 1, 1), 1, 0);
	tds_bg_push(tds_engine_global->bg_handle, 7, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_smallisland_640x640.png", 640, 640, 1, 1), 1, 0);
	tds_bg_push(tds_engine_global->bg_handle, 9, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_oceanlower_640x640.png", 640, 640, 1, 1), 1, 0);

	tds_render_set_ambient_brightness(tds_engine_global->render_handle, 1.0f);
}

void obj_world_day_destroy(struct tds_object* ptr) {
}

void obj_world_day_update(struct tds_object* ptr) {
}

void obj_world_day_draw(struct tds_object* ptr) {
}

void obj_world_day_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
