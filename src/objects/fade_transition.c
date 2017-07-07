#include "fade_transition.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
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

	data->activated = 0;

	int* saveid = tds_object_get_ipart(ptr, HUNTER_FADET_INDEX_SAVEID);

	if (saveid) {
		data->saveid = *saveid;
	} else {
		tds_logf(TDS_LOG_WARNING, "No saveID passed to fade_transition!\n");
		data->saveid = 0;
	}

	data->dest_world = tds_object_get_spart(ptr, HUNTER_FADET_INDEX_DEST);

	if (!data->dest_world) {
		tds_logf(TDS_LOG_WARNING, "No destination world set for fade transition. This trigger will be ignored.\n");
	}
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
		tds_engine_broadcast(tds_engine_global, MSG_FADE_REQ_BLACK, ptr);
	}
}

void obj_fade_transition_draw(struct tds_object* ptr) {}

void obj_fade_transition_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_fade_transition_data* data = (struct obj_fade_transition_data*) ptr->object_data;

	switch (msg) {
	case MSG_FADE_BLACK:
		if (param == ptr) { /* We called the fade and it completed. */
			tds_logf(TDS_LOG_DEBUG, "triggering world load, dest [%s]\n", data->dest_world);
			tds_savestate_set(tds_engine_global->savestate_handle, HUNTER_SAVE_SPAWN_ID, &data->saveid, sizeof data->saveid);
			tds_savestate_set(tds_engine_global->savestate_handle, HUNTER_SAVE_WORLD_NAME, data->dest_world, strlen(data->dest_world));
			tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_SAVE_ALL, NULL); /* save elevator states */
			tds_engine_broadcast(tds_engine_global, MSG_COINPILE_SAVE_ALL, NULL); /* save coinpile states */
			tds_savestate_write(tds_engine_global->savestate_handle);
			tds_engine_request_load(tds_engine_global, data->dest_world);
		}
		break;
	case MSG_FADE_START_BLACK:
		if (param == ptr) {
			data->activated = 1;
		}
		break;
	}
}
