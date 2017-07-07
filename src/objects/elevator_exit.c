#include "elevator_exit.h"
#include "camera.h"
#include "player.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

/* This is pretty much a complete copy of trigger_dialog, except with an index for a sprite. */

struct tds_object_type obj_elevator_exit_type = {
	.type_name = "obj_elevator_exit",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_elevator_exit_data),
	.func_init = obj_elevator_exit_init,
	.func_destroy = obj_elevator_exit_destroy,
	.func_update = obj_elevator_exit_update,
	.func_draw = obj_elevator_exit_draw,
	.func_msg = obj_elevator_exit_msg,
	.save = 1,
};

void obj_elevator_exit_init(struct tds_object* ptr) {
	struct obj_elevator_exit_data* data = (struct obj_elevator_exit_data*) ptr->object_data;
	data->index = *tds_object_get_ipart(ptr, HUNTER_ELEVATOR_EXIT_INDEX_STOPINDEX);
}

void obj_elevator_exit_destroy(struct tds_object* ptr) {
}

void obj_elevator_exit_update(struct tds_object* ptr) {
	struct obj_elevator_exit_data* data = (struct obj_elevator_exit_data*) ptr->object_data;
}

void obj_elevator_exit_draw(struct tds_object* ptr) {
	struct obj_elevator_exit_data* data = (struct obj_elevator_exit_data*) ptr->object_data;
}

void obj_elevator_exit_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_elevator_exit_data* data = (struct obj_elevator_exit_data*) ptr->object_data;
}
