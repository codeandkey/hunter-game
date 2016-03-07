#include <tds/tds.h>
#include <tds/memory.h>

#include "tds_game/game_input.h"
#include "objects/objects.h"

#include "save.h"

#define HUNTER_CONFIG_FILENAME "hunter.cfg"

#include <string.h>
#include <unistd.h>

static void _load_sounds(struct tds_sound_cache* sndc_handle);
static void _load_sprites(struct tds_sprite_cache* sc_handle, struct tds_texture_cache* tc_handle);
static void _load_object_types(struct tds_object_type_cache* otc_handle);
static void _load_block_types(struct tds_block_map* block_map_handle, struct tds_texture_cache* tc_handle);

static char* _get_level_load(int index);

int main(int argc, char** argv) {
	struct tds_engine_desc desc = {0};
	struct tds_script* game_config = tds_script_create(HUNTER_CONFIG_FILENAME);

	int save_index = tds_script_get_var_int(game_config, "save", 0);
	tds_script_free(game_config);

	char* map_filename = _get_level_load(save_index);

	int arg = 0;
	while ((arg = getopt(argc, argv, "n")) != -1) {
		switch (arg) {
		case 'n':
			tds_free(map_filename);
			map_filename = NULL;
			break;
		}
	}

	desc.config_filename = "tds.cfg";
	desc.map_filename = map_filename;
	desc.save_index = save_index;
	desc.game_input = hunter_get_game_input();
	desc.game_input_size = hunter_get_game_input_size();

	desc.func_load_sounds = _load_sounds;
	desc.func_load_sprites = _load_sprites;
	desc.func_load_object_types = _load_object_types;
	desc.func_load_block_map = _load_block_types;

	struct tds_engine* engine_handle = tds_engine_create(desc);
	tds_logf(TDS_LOG_MESSAGE, "Starting engine.\n");
	tds_engine_run(engine_handle);
	tds_engine_free(engine_handle);

	tds_free(map_filename);
	tds_memcheck();

	return 0;
}

void _load_sounds(struct tds_sound_cache* sndc_handle) {
	tds_logf(TDS_LOG_MESSAGE, "Loading sounds.\n");
}

void _load_sprites(struct tds_sprite_cache* sc_handle, struct tds_texture_cache* tc_handle) {
	tds_sprite_cache_add(sc_handle, "font_debug", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/fonts/debug.png", 32, 32, 0, 0), 0.25f, 0.25f, 0.0f));

	tds_sprite_cache_add(sc_handle, "spr_editor_cursor", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/editor_cursor.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_editor_selector", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/editor_selector.png", 64, 32, 0, 0), 2.0f, 1.0f, 0.0f));

	tds_sprite_cache_add(sc_handle, "spr_player_idle_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_idle_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_idle_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_idle_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_look_up_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_look_up_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_look_up_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_look_up_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_hurt_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_hurt_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_hurt_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_hurt_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_walk_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_walk_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 100.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_walk_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_walk_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 100.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_jump_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_jump_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_jump_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_jump_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_fall_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_fall_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_fall_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_fall_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_ghost_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/ghost_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 150.0f));
	tds_sprite_cache_add(sc_handle, "spr_ghost_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/ghost_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 150.0f));
	tds_sprite_cache_add(sc_handle, "spr_savestation", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/save_station_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 150.0f));

	tds_logf(TDS_LOG_MESSAGE, "Loading sprites.\n");
}

void _load_object_types(struct tds_object_type_cache* otc_handle) {
	tds_logf(TDS_LOG_MESSAGE, "Loading object types.\n");

	tds_object_type_cache_add(otc_handle, "system", &obj_system_type);
	tds_object_type_cache_add(otc_handle, "env", &obj_env_type);
	tds_object_type_cache_add(otc_handle, "world_day", &obj_world_day_type);
	tds_object_type_cache_add(otc_handle, "world_night", &obj_world_night_type);
	tds_object_type_cache_add(otc_handle, "obj_player", &obj_player_type);
	tds_object_type_cache_add(otc_handle, "obj_ghost", &obj_ghost_type);
	tds_object_type_cache_add(otc_handle, "obj_savestation", &obj_savestation_type);
	tds_object_type_cache_add(otc_handle, "obj_light_dir", &obj_light_dir_type);
	tds_object_type_cache_add(otc_handle, "obj_light_point", &obj_light_point_type);
	tds_object_type_cache_add(otc_handle, "obj_attractor", &obj_attractor_type);
}

void _load_block_types(struct tds_block_map* block_map_handle, struct tds_texture_cache* tc_handle) {
	tds_logf(TDS_LOG_MESSAGE, "Loading block types.\n");

	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_001_grass.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 1);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_002_gstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 2);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_003_hmgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 3);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_004_lgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 4);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_005_vmgstone.png", 16, 16, 1, 0),TDS_BLOCK_TYPE_SOLID, 5);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_006_rgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 6);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_007_tgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 7);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_008_bgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 8);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_009_ctlgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 9);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_010_ctrgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 10);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_011_cblgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 11);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_012_cbrgstone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 12);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_013_dirt.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 13);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_014_mossdirt.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 14);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_015_moss.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_NOLIGHT, 15);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_016_rslope_grass.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_RTSLOPE | TDS_BLOCK_TYPE_SOLID, 16);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_017_lslope_grass.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_LTSLOPE | TDS_BLOCK_TYPE_SOLID, 17);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_018_rcorner_grass.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 18);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_019_lcorner_grass.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 19);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_020_stone.png", 16, 16, 1, 0), TDS_BLOCK_TYPE_SOLID, 20);
}

char* _get_level_load(int index) {
	struct tds_savestate* ss_ptr = tds_savestate_create();
	tds_savestate_set_index(ss_ptr, index);

	char* default_name = (char*) "default";

	struct tds_savestate_entry entry = tds_savestate_get(ss_ptr, HUNTER_SAVE_WORLD_NAME);

	if (!entry.data || !strlen(entry.data)) {
		tds_logf(TDS_LOG_WARNING, "Missing entry for world name. Using default..\n");
		tds_savestate_set(ss_ptr, HUNTER_SAVE_WORLD_NAME, default_name, strlen(default_name));
		tds_savestate_write(ss_ptr);
		tds_savestate_free(ss_ptr);
		return default_name;
	}

	char* output = tds_malloc(entry.data_len + 1);
	memcpy(output, entry.data, entry.data_len);
	output[entry.data_len] = 0;

	tds_savestate_free(ss_ptr);

	return output;
}
