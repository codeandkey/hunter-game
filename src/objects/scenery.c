#include "scenery.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

/* This is pretty much a complete copy of trigger_dialog, except with an index for a sprite. */

struct tds_object_type obj_scenery_type = {
	.type_name = "obj_scenery",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_scenery_data),
	.func_init = obj_scenery_init,
	.func_destroy = obj_scenery_destroy,
	.func_update = obj_scenery_update,
	.func_draw = obj_scenery_draw,
	.func_msg = obj_scenery_msg,
	.save = 1,
};

void obj_scenery_init(struct tds_object* ptr) {
	struct obj_scenery_data* data = (struct obj_scenery_data*) ptr->object_data;

	char* spr = tds_object_get_spart(ptr, HUNTER_SCENERY_INDEX_SPRITE);

	if (spr) {
		tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, spr));
		tds_object_anim_start(ptr);
	}

	data->name = tds_object_get_spart(ptr, HUNTER_SCENERY_INDEX_SEQUENCE);

	int* oneshot = tds_object_get_ipart(ptr, HUNTER_SCENERY_INDEX_ONESHOT);

	if (oneshot && data->name) { /* Force oneshot and deactivated if no name. */
		data->oneshot = *oneshot;
	} else {
		data->oneshot = 0;
	}

	int* use = tds_object_get_ipart(ptr, HUNTER_SCENERY_INDEX_USE);

	if (use) {
		data->use = *use;
	} else {
		data->use = 0;
	}

	data->activated = (data->name != NULL);
}

void obj_scenery_destroy(struct tds_object* ptr) {
}

void obj_scenery_update(struct tds_object* ptr) {
	struct obj_scenery_data* data = (struct obj_scenery_data*) ptr->object_data;

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) {
			tds_logf(TDS_LOG_WARNING, "Failed to locate player object. Will try again next frame.\n");
			return;
		}
	}

	if (tds_collision_get_overlap(ptr, data->player) && !data->use) {
		if (data->activated && data->name) {
			tds_logf(TDS_LOG_DEBUG, "Triggered, sending dialog seq for %s\n", data->name);
			tds_engine_broadcast(tds_engine_global, MSG_DIALOG_REQ_START, data->name);
			/* We don't deactivate here. We wait for confirmation from the dialog module. */
		}
	} else {
		if (!data->oneshot && !data->activated) {
			data->activated = 1;
		}
	}
}

void obj_scenery_draw(struct tds_object* ptr) {
}

void obj_scenery_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_scenery_data* data = (struct obj_scenery_data*) ptr->object_data;

	switch(msg) {
	case MSG_DIALOG_START:
		if (data->name && !strcmp((char*) param, data->name)) {
			/* Our dialog sequence started. Disable! */
			data->activated = 0;
		}
		break;
	case MSG_PLAYER_ACTION:
		if (data->use && data->player && data->activated && data->name && tds_collision_get_overlap(ptr, data->player)) {
			tds_engine_broadcast(tds_engine_global, MSG_DIALOG_REQ_START, data->name);
		}
		break;
	}
}
