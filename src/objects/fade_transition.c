#include "fade_transition.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../tds_game/game_msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_fade_transition_type = {
	.type_name = "obj_fade_transition",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_fade_transition_data),
	.func_init = obj_fade_transition_init,
	.func_destroy = obj_fade_transition_destroy,
	.func_update = obj_fade_transition_update,
	.func_draw = obj_fade_transition_draw,
	.func_msg = obj_fade_transition_msg,
	.save = 1,
};

void obj_fade_transition_init(struct tds_object* ptr) {
	struct obj_fade_transition_data* data = (struct obj_fade_transition_data*) ptr->object_data;

	data->slope = HUNTER_FADET_SLOPE_DEFAULT;
	data->fade_factor = 1.0f;
	data->activated = 0;

	float* slope = tds_object_get_fpart(ptr, HUNTER_FADET_INDEX_SLOPE);

	if (slope) {
		data->slope = *slope;
	}

	data->dest_world = tds_object_get_spart(ptr, HUNTER_FADET_INDEX_DEST);

	if (!data->dest_world) {
		tds_logf(TDS_LOG_WARNING, "No destination world set for fade transition. This trigger will be ignored.\n");
	}

	data->start_point = tds_clock_get_point();
}

void obj_fade_transition_destroy(struct tds_object* ptr) {
}

void obj_fade_transition_update(struct tds_object* ptr) {
	struct obj_fade_transition_data* data = (struct obj_fade_transition_data*) ptr->object_data;

	if (!data->dest_world) {
		return;
	}

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) {
			tds_logf(TDS_LOG_DEBUG, "Failed to find player object. Will look again next frame.\n");
			return;
		}
	}

	if (!data->activated && tds_collision_get_overlap(data->player, ptr)) {
		data->activated = 1;
		data->start_point = tds_clock_get_point();
	}

	if (data->activated) {
		data->fade_factor = 1.0f - tds_clock_get_ms(data->start_point) * data->slope;

		if (data->fade_factor < 0.0f) {
			tds_logf(TDS_LOG_DEBUG, "triggering world load, slope %f, factor %f, dest [%s]\n", data->slope, data->fade_factor, data->dest_world);
			tds_engine_request_load(tds_engine_global, data->dest_world);
		}
	}
}

void obj_fade_transition_draw(struct tds_object* ptr) {
	struct obj_fade_transition_data* data = (struct obj_fade_transition_data*) ptr->object_data;

	if (data->activated) {
		tds_render_set_fade_factor(tds_engine_global->render_handle, data->fade_factor);
	}
}

void obj_fade_transition_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
