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
	struct obj_rain_data* data = (struct obj_rain_data*) ptr->object_data;

	tds_effect_push(tds_engine_global->effect_handle, &effect_rain_type);
	tds_sound_source_load_buffer(ptr->snd_src, tds_sound_cache_get(tds_engine_global->sndc_handle, "bg_rain"));
	ptr->snd_loop = 1;
	tds_sound_source_play(ptr->snd_src);

	data->start_point = tds_clock_get_point();
	data->ms_interval = HUNTER_RAIN_LIGHTNING_MS_MIN + rand() % HUNTER_RAIN_LIGHTNING_MS_VAR;
}

void obj_rain_destroy(struct tds_object* ptr) {
}

void obj_rain_update(struct tds_object* ptr) {
	struct obj_rain_data* data = (struct obj_rain_data*) ptr->object_data;

	if (tds_clock_get_ms(data->start_point) > data->ms_interval) {
		data->ms_interval = HUNTER_RAIN_LIGHTNING_MS_MIN + rand() % HUNTER_RAIN_LIGHTNING_MS_VAR;
		data->lightning = HUNTER_RAIN_LIGHTNING_INT_MIN + ((rand() % HUNTER_RAIN_LIGHTNING_PRECISION) / (float) HUNTER_RAIN_LIGHTNING_PRECISION) * HUNTER_RAIN_LIGHTNING_INT_VAR;
		data->start_point = tds_clock_get_point();
	}

	data->lightning /= HUNTER_RAIN_LIGHTNING_DECAY;
}

void obj_rain_draw(struct tds_object* ptr) {
	struct obj_rain_data* data = (struct obj_rain_data*) ptr->object_data;
	struct tds_render_light lt = { TDS_RENDER_LIGHT_DIRECTIONAL, 0.0f, -1.0f, 0.0f, data->lightning / 2.0f, data->lightning, 0.0f, NULL };
	tds_render_submit_light(tds_engine_global->render_handle, lt);
}

void obj_rain_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
