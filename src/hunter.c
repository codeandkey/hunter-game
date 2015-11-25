#include <tds/tds.h>
#include <tds/memory.h>

#include "tds_game/game_input.h"
#include "objects/objects.h"

static void _load_sounds(struct tds_sound_cache* sndc_handle);
static void _load_sprites(struct tds_sprite_cache* sc_handle, struct tds_texture_cache* tc_handle);
static void _load_object_types(struct tds_object_type_cache* otc_handle);
static void _load_block_types(struct tds_block_map* block_map_handle, struct tds_texture_cache* tc_handle);

int main(int argc, char** argv) {
	struct tds_engine_desc desc = {0};

	desc.config_filename = "tds_hunter.cfg";
	desc.map_filename = "default";
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
	tds_sprite_cache_add(sc_handle, "spr_player_hurt_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_hurt_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_hurt_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_hurt_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_walk_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_walk_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 100.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_walk_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_walk_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 100.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_jump_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_jump_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_player_jump_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_jump_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));
	tds_sprite_cache_add(sc_handle, "spr_ghost_left", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/ghost_left_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 150.0f));
	tds_sprite_cache_add(sc_handle, "spr_ghost_right", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/ghost_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 150.0f));

	tds_logf(TDS_LOG_MESSAGE, "Loading sprites.\n");
}

void _load_object_types(struct tds_object_type_cache* otc_handle) {
	tds_logf(TDS_LOG_MESSAGE, "Loading object types.\n");

	tds_object_type_cache_add(otc_handle, "system", &obj_system_type);
	tds_object_type_cache_add(otc_handle, "obj_player", &obj_player_type);
	tds_object_type_cache_add(otc_handle, "obj_ghost", &obj_ghost_type);
}

void _load_block_types(struct tds_block_map* block_map_handle, struct tds_texture_cache* tc_handle) {
	tds_logf(TDS_LOG_MESSAGE, "Loading block types.\n");

	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_001_grass.png", 16, 16, 1, 0), 1, 1);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_002_gstone.png", 16, 16, 1, 0), 1, 2);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_003_hmgstone.png", 16, 16, 1, 0), 1, 3);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_004_lgstone.png", 16, 16, 1, 0), 1, 4);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_005_vmgstone.png", 16, 16, 1, 0), 1, 5);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_006_rgstone.png", 16, 16, 1, 0), 1, 6);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_007_tgstone.png", 16, 16, 1, 0), 1, 7);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_008_bgstone.png", 16, 16, 1, 0), 1, 8);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_009_ctlgstone.png", 16, 16, 1, 0), 1, 9);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_010_ctrgstone.png", 16, 16, 1, 0), 1, 10);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_011_cblgstone.png", 16, 16, 1, 0), 1, 11);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_012_cbrgstone.png", 16, 16, 1, 0), 1, 12);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_013_dirt.png", 16, 16, 1, 0), 1, 13);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_014_mossdirt.png", 16, 16, 1, 0), 1, 14);
	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_015_moss.png", 16, 16, 1, 0), 0, 15);
}
