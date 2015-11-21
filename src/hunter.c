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
	desc.map_filename = NULL;
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

	tds_sprite_cache_add(sc_handle, "spr_player", tds_sprite_create(tds_texture_cache_get(tc_handle, "res/sprites/player_idle_right_32x32.png", 32, 32, 0, 0), 1.0f, 1.0f, 0.0f));

	tds_logf(TDS_LOG_MESSAGE, "Loading sprites.\n");
}

void _load_object_types(struct tds_object_type_cache* otc_handle) {
	tds_logf(TDS_LOG_MESSAGE, "Loading object types.\n");

	tds_object_type_cache_add(otc_handle, "system", &obj_system_type);
	tds_object_type_cache_add(otc_handle, "obj_player", &obj_player_type);
}

void _load_block_types(struct tds_block_map* block_map_handle, struct tds_texture_cache* tc_handle) {
	tds_logf(TDS_LOG_MESSAGE, "Loading block types.\n");

	tds_block_map_add(block_map_handle, tds_texture_cache_get(tc_handle, "res/sprites/world_001_grass.png", 16, 16, 1, 1), 1, 1);
}
