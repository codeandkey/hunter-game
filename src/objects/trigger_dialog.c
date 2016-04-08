#include "trigger_dialog.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_trigger_dialog_type = {
	.type_name = "obj_trigger_dialog",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_trigger_dialog_data),
	.func_init = obj_trigger_dialog_init,
	.func_destroy = obj_trigger_dialog_destroy,
	.func_update = obj_trigger_dialog_update,
	.func_draw = obj_trigger_dialog_draw,
	.func_msg = obj_trigger_dialog_msg,
	.save = 1,
};

void obj_trigger_dialog_init(struct tds_object* ptr) {
	struct obj_trigger_dialog_data* data = (struct obj_trigger_dialog_data*) ptr->object_data;

	data->name = tds_object_get_spart(ptr, HUNTER_DIALOG_INDEX_SEQUENCE);

	int* oneshot = tds_object_get_ipart(ptr, HUNTER_DIALOG_INDEX_ONESHOT);

	if (oneshot) {
		data->oneshot = *oneshot;
	} else {
		data->oneshot = 0;
	}

	int* use = tds_object_get_ipart(ptr, HUNTER_DIALOG_INDEX_USE);

	if (use) {
		data->use = *use;
	} else {
		data->use = 0;
	}

	data->activated = 1;
}

void obj_trigger_dialog_destroy(struct tds_object* ptr) {
}

void obj_trigger_dialog_update(struct tds_object* ptr) {
	struct obj_trigger_dialog_data* data = (struct obj_trigger_dialog_data*) ptr->object_data;

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) {
			tds_logf(TDS_LOG_WARNING, "Failed to locate player object. Will try again next frame.\n");
			return;
		}
	}

	if (tds_collision_get_overlap(ptr, data->player) && !data->use) {
		if (data->activated) {
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

void obj_trigger_dialog_draw(struct tds_object* ptr) {
}

void obj_trigger_dialog_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_trigger_dialog_data* data = (struct obj_trigger_dialog_data*) ptr->object_data;

	switch(msg) {
	case MSG_DIALOG_START:
		if (!strcmp((char*) param, data->name)) {
			/* Our dialog sequence started. Disable! */
			data->activated = 0;
		}
		break;
	case MSG_PLAYER_ACTION:
		if (data->use && data->player && data->activated && tds_collision_get_overlap(ptr, data->player)) {
			tds_engine_broadcast(tds_engine_global, MSG_DIALOG_REQ_START, data->name);
		}
		break;
	}
}
