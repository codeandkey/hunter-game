#include "player.h"
#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../tds_game/game_msg.h"

#include "../save.h"

struct tds_object_type obj_player_type = {
	.type_name = "obj_player",
	.default_sprite = "spr_player_idle_right",
	.data_size = sizeof(struct obj_player_data),
	.func_init = obj_player_init,
	.func_destroy = obj_player_destroy,
	.func_update = obj_player_update,
	.func_draw = obj_player_draw,
	.func_msg = obj_player_msg,
	.save = 1
};

void obj_player_init(struct tds_object* ptr) {
	struct obj_player_data* data = (struct obj_player_data*) ptr->object_data;

	if (tds_engine_get_object_by_type(tds_engine_global, "obj_camera")) {
		tds_logf(TDS_LOG_WARNING, "There is already a camera which exists. This player will not create one.\n");
	} else {
		struct tds_object* camera = tds_object_create(&obj_camera_type, ptr->hmgr, ptr->smgr, ptr->x, ptr->y, 0.0f, NULL);
		tds_object_msg(camera, ptr, TDS_GAME_MSG_CAMERA_TRACK, ptr);
	}

	ptr->cbox_width = 0.3f;
	ptr->cbox_height = 0.9f;

	data->direction = 1;
	data->spawn_x = ptr->x;
	data->spawn_y = ptr->y;

	data->state_hit = data->state_hit_hurt = 0;
}

void obj_player_destroy(struct tds_object* ptr) {
}

void obj_player_update(struct tds_object* ptr) {
	/* We will not allow slower movement than normal, we will use the axis and implement a basic deadzone */
	struct obj_player_data* data = (struct obj_player_data*) ptr->object_data;

	int move_key_low = tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_MOVE_LEFT);
	int move_key_high = tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_MOVE_RIGHT);
	int move_axis = tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_AXIS_MOVEMENT);
	int key_lookup = tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_MOVE_UP);

	float movement_axis = tds_input_map_get_axis(tds_engine_global->input_map_handle, move_key_low, move_key_high, move_axis);

	if (movement_axis <= -HUNTER_PLAYER_MOVE_DEADZONE) {
		data->movement_direction = -1;
		data->direction = -1;
	} else if (movement_axis >= HUNTER_PLAYER_MOVE_DEADZONE) {
		data->movement_direction = 1;
		data->direction = 1;
	} else {
		data->movement_direction = 0;
	}

	/* We will first change the player's x and y speeds as necessary. */
	if (!data->state_hit_hurt) {
		ptr->xspeed += data->movement_direction * HUNTER_PLAYER_MOVE_ACCEL;
	}

	if (ptr->xspeed < -HUNTER_PLAYER_MOVE_MAXSPEED) {
		ptr->xspeed = -HUNTER_PLAYER_MOVE_MAXSPEED;
	}

	if (ptr->xspeed > HUNTER_PLAYER_MOVE_MAXSPEED) {
		ptr->xspeed = HUNTER_PLAYER_MOVE_MAXSPEED;
	}

	if (!data->movement_direction && !data->state_hit_hurt) {
		ptr->xspeed /= HUNTER_PLAYER_MOVE_DECEL;
	}

	ptr->yspeed += HUNTER_PLAYER_GRAVITY;

	/* Movement is addressed in a very special way which allows it to be smooth: we only act on XY collisions if X and Y both fail. */
	int collision_x = 0, collision_y = 0, collision_xy = 0;

	/* We will offset the player's position to test collisions. */

	float orig_x = ptr->x, orig_y = ptr->y;

	ptr->x = orig_x + ptr->xspeed;
	ptr->y = orig_y;

	float cx_x = 0.0f, cx_y = 0.0f, cx_w = 0.0f, cx_h = 0.0f;

	if (tds_world_get_overlap_fast(tds_engine_global->world_handle, ptr, &cx_x, &cx_y, &cx_w, &cx_h)) {
		collision_x = 1;
	}

	ptr->x = orig_x;
	ptr->y = orig_y + ptr->yspeed;

	if (tds_world_get_overlap_fast(tds_engine_global->world_handle, ptr, NULL, NULL, NULL, NULL)) {
		collision_y = 1;

		data->can_jump = (ptr->yspeed < 0.0f);
	} else {
		data->can_jump = 0;
	}

	ptr->x = orig_x + ptr->xspeed;
	ptr->y = orig_y + ptr->yspeed;

	if (tds_world_get_overlap_fast(tds_engine_global->world_handle, ptr, NULL, NULL, NULL, NULL)) {
		collision_xy = 1;
	}

	ptr->x = orig_x;
	ptr->y = orig_y; /* We reset the player's position before calculating autolift. This makes necessary teleportation easier. */

	if (collision_x) {
		float d = (cx_y + cx_h / 2.0f) - (ptr->y - ptr->cbox_height / 2.0f);

		if (d <= HUNTER_PLAYER_AUTOLIFT_DIST && d >= 0 && data->can_jump) {

			ptr->y = orig_y + d;
			ptr->x = orig_x + ptr->xspeed;

			if (!tds_world_get_overlap_fast(tds_engine_global->world_handle, ptr, NULL, NULL, NULL, NULL)) {
				//ptr->yspeed = 0.0f;
			} else {
				ptr->y = orig_y;
				ptr->xspeed = 0.0f;
			}

			ptr->x = orig_x;
		} else {
			ptr->xspeed = 0.0f;
		}
	}

	if (collision_y) {
		ptr->yspeed = 0.0f;
	}

	if (collision_xy && !collision_x && !collision_y) {
		ptr->xspeed = ptr->yspeed = 0.0f;
	}

	/* State transfers : We act on the player hit state. */
	if (data->can_jump) {
		data->state_hit_hurt = 0; // Stop the 'hurt' animation once we touch the ground.
	}

	if (data->state_hit && tds_clock_get_ms(data->timer_hit_recover) >= HUNTER_PLAYER_HIT_RECOVERY) {
		data->state_hit = 0;
	}

	if (data->state_hit && tds_clock_get_ms(data->timer_hit_flash) >= HUNTER_PLAYER_HIT_FLASH) {
		data->hit_flash = !data->hit_flash;
		data->timer_hit_flash = tds_clock_get_point();
	}

	if (!data->state_hit) {
		data->hit_flash = 0;
	}

	if (data->hit_flash) {
		ptr->a = 0.3f;
	} else {
		ptr->a = 1.0f;
	}

	if (data->can_jump) {
		data->look_up = tds_input_map_get_key(tds_engine_global->input_map_handle, key_lookup, 0);
	} else {
		data->look_up = 0;
	}
}

void obj_player_draw(struct tds_object* ptr) {
	struct obj_player_data* data = (struct obj_player_data*) ptr->object_data;

	/* Animation state switches! */
	if (data->state_hit_hurt) {
		if (data->direction > 0) {
			tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_hurt_right"));
		} else {
			tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_hurt_left"));
		}

		return;
	}

	if (data->can_jump) {
		if (data->movement_direction) {
			if (data->direction > 0) {
				tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_walk_right"));
			} else {
				tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_walk_left"));
			}
		} else {
			if (data->look_up) {
				if (data->direction > 0) {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_look_up_right"));
				} else {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_look_up_left"));
				}
			} else {
				if (data->direction > 0) {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_idle_right"));
				} else {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_idle_left"));
				}
			}
		}
	} else {
		if (ptr->yspeed > 0.0f) {
			if (data->direction > 0) {
				tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_jump_right"));
			} else {
				tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_jump_left"));
			}
		} else {
			if (data->direction > 0) {
				tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_fall_right"));
			} else {
				tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_fall_left"));
			}
		}
	}

	struct tds_render_light lt = {TDS_RENDER_LIGHT_POINT, ptr->x, ptr->y, 1.0f, 1.0f, 1.0f, 0.5f, NULL};
	tds_render_submit_light(tds_engine_global->render_handle, lt);
}

void obj_player_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_player_data* data = (struct obj_player_data*) ptr->object_data;
	struct tds_object* target_station = (struct tds_object*) param;
	struct tds_savestate_entry entry;
	int key = 0;

	switch (msg) {
	case TDS_GAME_MSG_PLAYER_HIT:
		if (!data->state_hit) {
			data->state_hit = data->state_hit_hurt = 1;
			data->timer_hit_recover = tds_clock_get_point();
			ptr->xspeed = (data->direction > 0) ? -HUNTER_PLAYER_HIT_VEL : HUNTER_PLAYER_HIT_VEL;
			ptr->yspeed = HUNTER_PLAYER_HIT_VEL;
			data->can_jump = 0;
		}
		break;
	case TDS_GAME_MSG_SAVESTATION_START:
		ptr->x = data->spawn_x = target_station->x;
		ptr->y = data->spawn_y = target_station->y;
		break;
	case TDS_MSG_KEY_PRESSED:
		key = *((int*) param);
		if (data->can_jump && key == tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_JUMP)) {
			ptr->yspeed += HUNTER_PLAYER_JUMP;
			data->can_jump = 0;
		}
		if (key == tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_RESET)) {
			ptr->x = data->spawn_x;
			ptr->y = data->spawn_y;
			ptr->xspeed = 0.0f;
			ptr->yspeed = 0.0f;
		}
		break;
	case TDS_MSG_MAP_READY:
		/* We hail any savestations that we might belong to. */
		entry = tds_savestate_get(tds_engine_global->savestate_handle, HUNTER_SAVE_SPAWN_ID);

		if (entry.data) {
			tds_logf(TDS_LOG_DEBUG, "Player received ready message. Querying for savestations.. Looking for spawn at ID %d\n", *((int*) entry.data));
			tds_engine_broadcast(tds_engine_global, TDS_GAME_MSG_SAVESTATION_QUERY, entry.data);
		} else {
			tds_logf(TDS_LOG_DEBUG, "Player is not querying for savestations, the savestate did not respond with spawn data.\n");
		}
		break;
	}
}
