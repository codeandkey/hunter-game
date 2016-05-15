#include "bullet_bolt.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../msg.h"

struct tds_object_type obj_bullet_bolt_type = {
	.type_name = "obj_bullet_bolt",
	.default_sprite = "spr_bullet_bolt",
	.data_size = sizeof(struct obj_bullet_bolt_data),
	.func_init = obj_bullet_bolt_init,
	.func_destroy = obj_bullet_bolt_destroy,
	.func_update = obj_bullet_bolt_update,
	.func_draw = obj_bullet_bolt_draw,
	.func_msg = obj_bullet_bolt_msg,
	.save = 0
};

static void _obj_bullet_bolt_callback(void* data, struct tds_object* obj);

void obj_bullet_bolt_init(struct tds_object* ptr) {
	struct obj_bullet_bolt_data* data = (struct obj_bullet_bolt_data*) ptr->object_data;

	float* damage = tds_object_get_fpart(ptr, HUNTER_BULLET_INDEX_DAMAGE);

	data->damage = damage ? *damage : HUNTER_BULLET_DAMAGE;

	float* dx = tds_object_get_fpart(ptr, HUNTER_BULLET_INDEX_DX);
	float* dy = tds_object_get_fpart(ptr, HUNTER_BULLET_INDEX_DY);

	ptr->xspeed = dx ? *dx : HUNTER_BULLET_DX;
	ptr->yspeed = dy ? *dy : HUNTER_BULLET_DY;

	float* r = tds_object_get_fpart(ptr, HUNTER_BULLET_INDEX_R);
	float* g = tds_object_get_fpart(ptr, HUNTER_BULLET_INDEX_G);
	float* b = tds_object_get_fpart(ptr, HUNTER_BULLET_INDEX_B);

	ptr->r = r ? *r : HUNTER_BULLET_R;
	ptr->g = g ? *g : HUNTER_BULLET_G;
	ptr->b = b ? *b : HUNTER_BULLET_B;
}

void obj_bullet_bolt_destroy(struct tds_object* ptr) {
}

void obj_bullet_bolt_update(struct tds_object* ptr) {
	struct obj_bullet_bolt_data* data = (struct obj_bullet_bolt_data*) ptr->object_data;

	if (tds_world_get_overlap_fast(tds_engine_get_foreground_world(tds_engine_global), ptr, NULL, NULL, NULL, NULL, TDS_BLOCK_TYPE_SOLID, TDS_BLOCK_TYPE_SOLID, 0)) {
		tds_object_free(ptr);
		return;
	}

	ptr->yspeed -= HUNTER_BULLET_GRAVITY;

	tds_engine_object_foreach(tds_engine_global, ptr, _obj_bullet_bolt_callback);
}

void obj_bullet_bolt_draw(struct tds_object* ptr) {
	struct obj_bullet_bolt_data* data = (struct obj_bullet_bolt_data*) ptr->object_data;

	struct tds_render_light lt = {
		TDS_RENDER_LIGHT_POINT,
		ptr->x, ptr->y,
		ptr->r, ptr->g, ptr->b,
		HUNTER_BULLET_LT_DIST,
		NULL
	};

	tds_render_submit_light(tds_engine_global->render_handle, lt);
}

void obj_bullet_bolt_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_bullet_bolt_data* data = (struct obj_bullet_bolt_data*) ptr->object_data;

	switch (msg) {
	case MSG_BULLET_SET_DAMAGE:
		data->damage = *((float*) param);
		break;
	case MSG_BULLET_SET_DX:
		ptr->xspeed = *((float*) param);
		break;
	case MSG_BULLET_SET_DY:
		ptr->yspeed = *((float*) param);
		break;
	case MSG_BULLET_SET_R:
		ptr->r = *((float*) param);
		break;
	case MSG_BULLET_SET_G:
		ptr->g = *((float*) param);
		break;
	case MSG_BULLET_SET_B:
		ptr->b = *((float*) param);
		break;
	case MSG_BULLET_HIT:
		tds_object_free(ptr);
		break;
	}
}

void _obj_bullet_bolt_callback(void* userdata, struct tds_object* obj) {
	struct tds_object* ptr = (struct tds_object*) userdata;
	struct obj_bullet_bolt_data* data = (struct obj_bullet_bolt_data*) ptr->object_data;

	if (tds_collision_get_overlap(ptr, obj)) {
		tds_object_msg(obj, ptr, MSG_BULLET_REQ_HIT, &data->damage);
	}
}
