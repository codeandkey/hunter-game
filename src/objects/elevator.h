#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_ELEVATOR_INDEX_STOP1 0 /* string: global object id of the first stop location */
#define HUNTER_ELEVATOR_INDEX_STOP2 1 /* string: ^ for the second stop */
#define HUNTER_ELEVATOR_INDEX_SAVE 2  /* savestate index for saving states */

#define HUNTER_ELEVATOR_STATE_PREMOVE 0
#define HUNTER_ELEVATOR_STATE_MOVE 1
#define HUNTER_ELEVATOR_STATE_POSTMOVE 2
#define HUNTER_ELEVATOR_STATE_IDLE 3

#define HUNTER_ELEVATOR_REST_TIME 1000
#define HUNTER_ELEVATOR_SPEED 0.05f

/* the elevator object will start at the closest of the first two stops. */
/* the elevator stops will trigger with the player and teleport the player to the elevator. then the elevator will move to the other stop and wait there. */
/* after stopping, the elevator teleports the player to the nearest elevator exit point */

/* elevator exits of of type obj_elevator_exit, elevator stops are of type obj_elevator_stop */
/* the elevator will actually stop directly BELOW the stop location. so make room for it. */

struct tds_object_type obj_elevator_type;

void obj_elevator_init(struct tds_object* ptr);
void obj_elevator_destroy(struct tds_object* ptr);
void obj_elevator_update(struct tds_object* ptr);
void obj_elevator_draw(struct tds_object* ptr);
void obj_elevator_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_elevator_data {
	struct tds_object* player;
	int index_stop1, index_stop2, at_stop1, state, save;
	struct tds_object* stop1, *stop2;
	tds_clock_point rest_timer;
};
