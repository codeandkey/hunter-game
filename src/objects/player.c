#include "player.h"
#include "player_camera.h"
#include "elevator_door.h"
#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../msg.h"

#include "../save.h"

#include <stdio.h>

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

	struct tds_object* pc = tds_object_create(&obj_player_camera_type, ptr->hmgr, ptr->smgr, ptr->x, ptr->y, 0.0f, NULL);

	ptr->cbox_width = 0.3f;
	ptr->cbox_height = 0.9f;

	data->direction = 1;
	data->spawn_x = ptr->x;
	data->spawn_y = ptr->y;
	data->input_enabled = 1;

	data->state_hit = data->state_hit_hurt = 0;
	data->collision_x = data->collision_y = data->collision_xy = 0;
	data->collision_slope = 0;

	ptr->layer = 10;
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

	ptr->cbox_width = 0.3f;
	ptr->cbox_height = 0.9f;

	if (movement_axis <= -HUNTER_PLAYER_MOVE_DEADZONE) {
		data->movement_direction = -1;
		data->direction = -1;
	} else if (movement_axis >= HUNTER_PLAYER_MOVE_DEADZONE) {
		data->movement_direction = 1;
		data->direction = 1;
	} else {
		data->movement_direction = 0;
	}

	if (!data->input_enabled) {
		data->movement_direction = 0;
	}

	data->collision_x = data->collision_y = data->collision_xy = 0;

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

	/* We will offset the player's position to test collisions. */

	float orig_x = ptr->x, orig_y = ptr->y;

	ptr->x = orig_x + ptr->xspeed;
	ptr->y = orig_y;

	float cx_x = 0.0f, cx_y = 0.0f, cx_w = 0.0f, cx_h = 0.0f;
	float cy_x = 0.0f, cy_y = 0.0f, cy_w = 0.0f, cy_h = 0.0f;

	int slopes = TDS_BLOCK_TYPE_RTSLOPE | TDS_BLOCK_TYPE_LTSLOPE;

	if (tds_world_get_overlap_fast(tds_engine_get_foreground_world(tds_engine_global), ptr, &cx_x, &cx_y, &cx_w, &cx_h, TDS_BLOCK_TYPE_SOLID, TDS_BLOCK_TYPE_SOLID, slopes)) {
		data->collision_x = 1;
	}

	struct tds_engine_object_list edoor_list = tds_engine_get_object_list_by_type(tds_engine_global, "obj_elevator_door");
	for (int i = 0; i < edoor_list.size; ++i) {
		struct obj_elevator_door_data* edoor_data = (struct obj_elevator_door_data*) edoor_list.buffer[i]->object_data;

		if (!edoor_data->opened && tds_collision_get_overlap(ptr, edoor_list.buffer[i])) {
			data->collision_x = 1;
		}
	}

	ptr->x = orig_x;
	ptr->y = orig_y + ptr->yspeed;

	if (tds_world_get_overlap_fast(tds_engine_get_foreground_world(tds_engine_global), ptr, &cy_x, &cy_y, &cy_w, &cy_h, TDS_BLOCK_TYPE_SOLID, TDS_BLOCK_TYPE_SOLID, slopes)) {
		data->collision_y = 1;

		data->can_jump = (ptr->yspeed < 0.0f);
	} else {
		data->can_jump = 0;
	}

	ptr->x = orig_x + ptr->xspeed;
	ptr->y = orig_y + ptr->yspeed;

	if (tds_world_get_overlap_fast(tds_engine_get_foreground_world(tds_engine_global), ptr, NULL, NULL, NULL, NULL, TDS_BLOCK_TYPE_SOLID, TDS_BLOCK_TYPE_SOLID, slopes)) {
		data->collision_xy = 1;
	}

	float slope_x, slope_y, slope_w, slope_h;
	int slope_flags = 0;
	data->should_correct = 0;
	data->collision_slope = 0;

	if ((slope_flags = tds_world_get_overlap_fast(tds_engine_get_foreground_world(tds_engine_global), ptr, &slope_x, &slope_y, &slope_w, &slope_h, 0, slopes, 0))) {
		/* Potential slope intersection. We don't ect until we're sure. */

		float slope_l = slope_x - slope_w / 2.0f, slope_r = slope_l + slope_w, slope_b = slope_y - slope_h / 2.0f, slope_t = slope_b + slope_h;

		data->csx = slope_x;
		data->csy = slope_y;
		data->csw = slope_w;
		data->csh = slope_h;

		if (slope_flags & TDS_BLOCK_TYPE_RTSLOPE && ptr->x - ptr->cbox_width / 2.0f >= slope_l - HUNTER_PLAYER_SLOPE_PADDING && ptr->x - ptr->cbox_width / 2.0f <= slope_r + HUNTER_PLAYER_SLOPE_PADDING && ptr->y - ptr->cbox_height / 2.0f >= slope_y - slope_h / 2.0f - HUNTER_PLAYER_SLOPE_PADDING) {
			float ty = (1.0f - (((ptr->x - ptr->cbox_width / 2.0f) - slope_l) / slope_w)) * slope_h + slope_b;

			ty = fmin(ty, slope_y + slope_h / 2.0f);
			data->should_correct = ty;

			if (ptr->y - ptr->cbox_height / 2.0f <= ty) {
				if (ptr->xspeed < 0) {
					ptr->yspeed = -ptr->xspeed;
					data->collision_slope = 1;
				} else {
					ptr->yspeed = 0.0f;
				}

				data->can_jump = 1;
			}
		}

		if (slope_flags & TDS_BLOCK_TYPE_LTSLOPE && ptr->x + ptr->cbox_width / 2.0f >= slope_l - HUNTER_PLAYER_SLOPE_PADDING && ptr->x + ptr->cbox_width / 2.0f <= slope_r + HUNTER_PLAYER_SLOPE_PADDING && ptr->y - ptr->cbox_height / 2.0f >= slope_y - slope_h / 2.0f - HUNTER_PLAYER_SLOPE_PADDING) {
			float ty = (((ptr->x + ptr->cbox_width / 2.0f) - slope_l) / slope_w) * slope_h + slope_b;

			ty = fmin(ty, slope_y + slope_h / 2.0f);

			data->should_correct = ty;

			if (ptr->y - ptr->cbox_height / 2.0f <= ty) {
				if (ptr->xspeed > 0) {
					ptr->yspeed = ptr->xspeed;
					data->collision_slope = 1;
				} else {
					ptr->yspeed = 0.0f;
				}

				data->can_jump = 1;
			}
		}
	}

	/* now, test for potential early slope collisions and allow the player to jump before he normally would */
	if (!data->can_jump) {
		/* test if we're falling towards a slope */
		ptr->y -= HUNTER_PLAYER_EARLY_SLOPE_REJUMP;

		if ((slope_flags = tds_world_get_overlap_fast(tds_engine_get_foreground_world(tds_engine_global), ptr, &slope_x, &slope_y, &slope_w, &slope_h, 0, slopes, 0))) {
			/* Potential slope intersection. We don't ect until we're sure. */
	
			float slope_l = slope_x - slope_w / 2.0f, slope_r = slope_l + slope_w, slope_b = slope_y - slope_h / 2.0f, slope_t = slope_b + slope_h;
	
			data->csx = slope_x;
			data->csy = slope_y;
			data->csw = slope_w;
			data->csh = slope_h;

			data->can_jump = 1;
	
			if (slope_flags & TDS_BLOCK_TYPE_RTSLOPE && ptr->x - ptr->cbox_width / 2.0f >= slope_l - HUNTER_PLAYER_SLOPE_PADDING && ptr->x - ptr->cbox_width / 2.0f <= slope_r + HUNTER_PLAYER_SLOPE_PADDING && ptr->y - ptr->cbox_height / 2.0f >= slope_y - slope_h / 2.0f - HUNTER_PLAYER_SLOPE_PADDING) {
				float ty = (1.0f - (((ptr->x - ptr->cbox_width / 2.0f) - slope_l) / slope_w)) * slope_h + slope_b;
	
				ty = fmin(ty, slope_y + slope_h / 2.0f);
	
				if (ptr->y - ptr->cbox_height / 2.0f <= ty) {
					data->can_jump = 1;
				}
			}
	
			if (slope_flags & TDS_BLOCK_TYPE_LTSLOPE && ptr->x + ptr->cbox_width / 2.0f >= slope_l - HUNTER_PLAYER_SLOPE_PADDING && ptr->x + ptr->cbox_width / 2.0f <= slope_r + HUNTER_PLAYER_SLOPE_PADDING && ptr->y - ptr->cbox_height / 2.0f >= slope_y - slope_h / 2.0f - HUNTER_PLAYER_SLOPE_PADDING) {
				float ty = (((ptr->x + ptr->cbox_width / 2.0f) - slope_l) / slope_w) * slope_h + slope_b;
	
				ty = fmin(ty, slope_y + slope_h / 2.0f);
	
				if (ptr->y - ptr->cbox_height / 2.0f <= ty) {
					data->can_jump = 1;
				}
			}
		}
	}

	ptr->x = orig_x;
	ptr->y = orig_y;

	if (data->collision_x) {
		ptr->xspeed = 0.0f;
	}

	if (data->collision_y && !data->collision_slope) {
		ptr->yspeed = 0.0f;
	}

	if (data->collision_xy && !data->collision_x && !data->collision_y) {
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

	if (data->pre_look_up) {
		if (!tds_input_map_get_key(tds_engine_global->input_map_handle, key_lookup, 0)) {
			data->pre_look_up = 0;
			data->look_up = 0;
		}

		data->look_up = tds_input_map_get_key(tds_engine_global->input_map_handle, key_lookup, 0) && tds_clock_get_ms(data->lookup_cp) > HUNTER_PLAYER_LOOKUP_DELAY;
	}

	if (data->in_elevator) {
		data->can_jump = 1;
		ptr->xspeed = 0.0f;
		ptr->yspeed = 0.0f;
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
			if (data->pre_look_up) {
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
		if (data->direction > 0) {
			if (ptr->yspeed > HUNTER_PLAYER_MIN_JUMP_ANIM_YSPEED) {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_jump_right"));
			} else if (ptr->yspeed < HUNTER_PLAYER_MAX_FALL_ANIM_YSPEED) {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_fall_right"));
			}
		} else {
			if (ptr->yspeed > HUNTER_PLAYER_MIN_JUMP_ANIM_YSPEED) {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_jump_left"));
			} else if (ptr->yspeed < HUNTER_PLAYER_MAX_FALL_ANIM_YSPEED) {
					tds_object_set_sprite(ptr, tds_sprite_cache_get(tds_engine_global->sc_handle, "spr_player_fall_left"));
			}
		}
	}

	/* debug cbox drawing */
	/*
	tds_render_flat_set_mode(tds_engine_global->render_flat_overlay_handle, TDS_RENDER_COORD_WORLDSPACE);
	if (data->can_jump) {
		if (!ptr->yspeed) {
			tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 0.0f, 1.0f, 0.0f, 0.5f);
		} else {
			tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 0.0f, 1.0f, 1.0f, 0.5f);
		}
	} else {
		tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 1.0f, 0.0f, 0.0f, 0.5f);
	}
	tds_render_flat_quad(tds_engine_global->render_flat_overlay_handle, ptr->x + ptr->xspeed - ptr->cbox_width / 2.0f, ptr->x + ptr->xspeed + ptr->cbox_width / 2.0f, ptr->y + ptr->yspeed - HUNTER_PLAYER_EARLY_SLOPE_REJUMP + ptr->cbox_height / 2.0f, ptr->y + ptr->yspeed - HUNTER_PLAYER_EARLY_SLOPE_REJUMP - ptr->cbox_height / 2.0f, NULL);
	*/
}

void obj_player_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_player_data* data = (struct obj_player_data*) ptr->object_data;
	struct tds_object* target_station = (struct tds_object*) param;
	struct tds_savestate_entry entry;
	int key = 0;

	switch (msg) {
	case MSG_PLAYER_HIT:
		if (!data->state_hit) {
			data->state_hit = data->state_hit_hurt = 1;
			data->timer_hit_recover = tds_clock_get_point();
			ptr->xspeed = (data->direction > 0) ? -HUNTER_PLAYER_HIT_VEL : HUNTER_PLAYER_HIT_VEL;
			ptr->yspeed = HUNTER_PLAYER_HIT_VEL;
			data->can_jump = 0;
		}
		break;
	case MSG_SAVESTATION_START:
		ptr->x = data->spawn_x = target_station->x;
		ptr->y = data->spawn_y = target_station->y;
		break;
	case TDS_MSG_KEY_PRESSED:
		key = *((int*) param);
		if (data->can_jump && key == tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_JUMP) && data->input_enabled) {
			ptr->yspeed = HUNTER_PLAYER_JUMP;
			data->can_jump = 0;
		}
		if (key == tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_JUMP) && !data->input_enabled) {
			tds_engine_broadcast(tds_engine_global, MSG_DIALOG_KP, NULL);
		}
		if (key == tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_RESET) && data->input_enabled) {
			ptr->x = data->spawn_x;
			ptr->y = data->spawn_y;
			ptr->xspeed = 0.0f;
			ptr->yspeed = 0.0f;
		}
		if (key == tds_key_map_get(tds_engine_global->key_map_handle, TDS_GAME_INPUT_MOVE_UP)) {
			tds_engine_broadcast(tds_engine_global, MSG_PLAYER_ACTION, ptr);
			if (!data->movement_direction && data->can_jump) {
				data->pre_look_up = 1;
				data->lookup_cp = tds_clock_get_point();
			}
		}
		break;
	case TDS_MSG_MAP_READY:
		/* We hail any savestations that we might belong to. */
		entry = tds_savestate_get(tds_engine_global->savestate_handle, HUNTER_SAVE_SPAWN_ID);

		if (entry.data) {
			tds_logf(TDS_LOG_DEBUG, "Player received ready message. Querying for savestations.. Looking for spawn at ID %d\n", *((int*) entry.data));
			tds_engine_broadcast(tds_engine_global, MSG_SAVESTATION_QUERY, entry.data);
		} else {
			tds_logf(TDS_LOG_DEBUG, "Player is not querying for savestations, the savestate did not respond with spawn data.\n");
		}
		break;
	case MSG_DIALOG_START:
		data->input_enabled = 0;
		break;
	case MSG_DIALOG_STOP:
		data->input_enabled = 1;
		break;
	case MSG_ELEVATOR_START_SEQ:
		data->in_elevator = 1;
		data->input_enabled = 0;
		break;
	case MSG_ELEVATOR_STOP_SEQ:
		data->in_elevator = 0;
		data->input_enabled = 1;
		break;
	}
}
