#include "rain.h"
#include "camera.h"

#include "../effects/effect_rain.h"

#include <tds/tds.h>

#include "../tds_game/game_msg.h"

#include <stdlib.h>

struct tds_object_type obj_rain_type = {
	.type_name = "obj_rain",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_rain_data),
	.func_init = obj_rain_init,
	.func_destroy = obj_rain_destroy,
	.func_update = obj_rain_update,
	.func_draw = obj_rain_draw,
	.func_msg = obj_rain_msg,
	.save = 1,
};

void obj_rain_init(struct tds_object* ptr) {
	/* The only purpose of this object is to push rain to the effect tracker. */

	tds_effect_push(tds_engine_global->effect_handle, &effect_rain_type);
	tds_sound_source_load_buffer(ptr->snd_src, tds_sound_cache_get(tds_engine_global->sndc_handle, "bg_rain"));
	ptr->snd_loop = 1;
	tds_sound_source_play(ptr->snd_src);
}

void obj_rain_destroy(struct tds_object* ptr) {
}

void obj_rain_update(struct tds_object* ptr) {
}

void obj_rain_draw(struct tds_object* ptr) {
}

void obj_rain_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
