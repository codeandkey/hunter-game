#include "fade_in.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../tds_game/game_msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_fade_in_type = {
	.type_name = "obj_fade_in",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_fade_in_data),
	.func_init = obj_fade_in_init,
	.func_destroy = obj_fade_in_destroy,
	.func_update = obj_fade_in_update,
	.func_draw = obj_fade_in_draw,
	.func_msg = obj_fade_in_msg,
	.save = 1,
};

void obj_fade_in_init(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	/* The purpose of this object is to do everything necessary to set up the "night" environment.
	 * We add some backgrounds and set the ambient light level. */

	data->factor = 0.0f;
	data->target_factor = 1.0f;
	data->anim_factor = HUNTER_FADEIN_ANIM_FACTOR;

	float* af = tds_object_get_fpart(ptr, HUNTER_FADEIN_INDEX_ANIM_FACTOR);

	if (af) {
		data->anim_factor = *af;
	}

	tds_render_set_fade_factor(tds_engine_global->render_handle, data->factor);
}

void obj_fade_in_destroy(struct tds_object* ptr) {
}

void obj_fade_in_update(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;
	data->factor = (data->target_factor - data->factor) / data->anim_factor + data->factor;
}

void obj_fade_in_draw(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;
	tds_render_set_fade_factor(tds_engine_global->render_handle, data->factor);
}

void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
