#pragma once

#include <tds/key_map.h>

struct tds_key_map_template* hunter_get_game_input(void);
int hunter_get_game_input_size(void);

/* Declare all game inputs here, define them in the C source. */

extern int TDS_GAME_INPUT_QUIT;
extern int TDS_GAME_INPUT_MOVE_LEFT;
extern int TDS_GAME_INPUT_MOVE_RIGHT;
extern int TDS_GAME_INPUT_MOVE_UP;
extern int TDS_GAME_INPUT_MOVE_DOWN;
extern int TDS_GAME_INPUT_JUMP;
extern int TDS_GAME_INPUT_ATTACK;

extern int TDS_GAME_INPUT_AXIS_MOVEMENT;
