#pragma once
#include <tds/object.h>

/*
 * obj_bullet_bolt
 * created by most weapons
 * not recommended to be created by maps.. it will be rogue (no owner so it will fly to the right and hit anything)
 */

#define HUNTER_BULLET_INDEX_DAMAGE 0 /* parameter for damage but there's really no point */
#define HUNTER_BULLET_INDEX_DX     1 /* xspeed */
#define HUNTER_BULLET_INDEX_DY     2 /* yspeed */
#define HUNTER_BULLET_INDEX_R      3
#define HUNTER_BULLET_INDEX_G      4
#define HUNTER_BULLET_INDEX_B      5

#define HUNTER_BULLET_DAMAGE 0.6f
#define HUNTER_BULLET_LT_DIST 10.0f
#define HUNTER_BULLET_DX 0.2f
#define HUNTER_BULLET_DY 0.0f
#define HUNTER_BULLET_R  0.1f
#define HUNTER_BULLET_G  0.5f
#define HUNTER_BULLET_B  0.1f

#define HUNTER_BULLET_GRAVITY 0.0001f

struct tds_object_type obj_bullet_bolt_type;

void obj_bullet_bolt_init(struct tds_object* ptr);
void obj_bullet_bolt_destroy(struct tds_object* ptr);
void obj_bullet_bolt_update(struct tds_object* ptr);
void obj_bullet_bolt_draw(struct tds_object* ptr);
void obj_bullet_bolt_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_bullet_bolt_data {
	float damage;
};
