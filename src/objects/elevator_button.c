#include "elevator_button.h"
#include "camera.h"
#include "player.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_elevator_button_type = {
	.type_name = "obj_elevator_button",
	.default_sprite = "spr_button_off",
	.data_size = sizeof(struct obj_elevator_button_data),
	.func_init = obj_elevator_button_init,
	.func_destroy = obj_elevator_button_destroy,
	.func_update = obj_elevator_button_update,
	.func_draw = obj_elevator_button_draw,
	.func_msg = obj_elevator_button_msg,
	.save = 1,
};

void obj_elevator_button_init(struct tds_object* ptr) {
	struct obj_elevator_button_data* data = (struct obj_elevator_button_data*) ptr->object_data;
	data->index = *tds_object_get_ipart(ptr, HUNTER_ELEVATOR_BUTTON_INDEX_STOPINDEX);
	data->used = 0;
	data->active = 1;

	data->src = tds_sound_source_create();
	tds_sound_source_load_buffer(data->src, tds_sound_cache_get(tds_engine_global->sndc_handle, "fx_button"));
	tds_sound_source_set_pos(data->src, ptr->pos.x / 16.0f, ptr->pos.y / 16.0f);
}

void obj_elevator_button_destroy(struct tds_object* ptr) {
	struct obj_elevator_button_data* data = (struct obj_elevator_button_data*) ptr->object_data;
	tds_sound_source_free(data->src);
}

void obj_elevator_button_update(struct tds_object* ptr) {
	struct obj_elevator_button_data* data = (struct obj_elevator_button_data*) ptr->object_data;

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");
	}
}

void obj_elevator_button_draw(struct tds_object* ptr) {
	struct obj_elevator_button_data* data = (struct obj_elevator_button_data*) ptr->object_data;

	if (data->used) {
		tds_object_set_sprite(ptr, tds_sprite_cache_get(ptr->smgr, "spr_button_on"));
	} else {
		tds_object_set_sprite(ptr, tds_sprite_cache_get(ptr->smgr, "spr_button_off"));
	}
}

void obj_elevator_button_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_elevator_button_data* data = (struct obj_elevator_button_data*) ptr->object_data;

	if (!data->player) return;

	switch (msg) {
	case MSG_PLAYER_ACTION:
		if (data->active && !data->used && tds_collision_get_overlap(ptr, data->player)) {
			data->used = 1;
			tds_sound_source_stop(data->src);
			tds_sound_source_play(data->src);
		}
		break;
	}
}
