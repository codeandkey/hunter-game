#pragma once

enum hunter_msg {
	/* obj_camera */

	MSG_CAMERA_TRACK,
	MSG_CAMERA_SHAKE,
	MSG_CAMERA_SIZE,

	/* obj_player */

	MSG_PLAYER_HIT,
	MSG_PLAYER_ACTION,

	/* obj_savestation */

	MSG_SAVESTATION_START,
	MSG_SAVESTATION_END,
	MSG_SAVESTATION_QUERY,

	/* used by env, deprecated */

	MSG_WORLD_READY,

	/* mod_bgm */

	MSG_BGM_REQ_START,
	MSG_BGM_REQ_STOP,
	MSG_BGM_START, /* broadcasted FROM bgm when bgm is actually started -- do NOT send */
	MSG_BGM_STOP, /* ^ for stopped */

	/* mod_dialog */

	MSG_DIALOG_REQ_START,
	MSG_DIALOG_REQ_STOP,
	MSG_DIALOG_KP, /* broadcast whenever you want dialog to advance */
	MSG_DIALOG_START, /* don't send these.. only dialog will */
	MSG_DIALOG_STOP,

	/* mod_fade messages */

	MSG_FADE_REQ_BLACK, /* param: pointer to an object to reply with MSG_FADE_RESPONSE */
	MSG_FADE_REQ_NONE, /* param: pointer to an object to reply with MSG_FADE_RESPONSE */
	MSG_FADE_REQ_TEXT, /* param: pointer to a tds_string or NULL */
	MSG_FADE_BLACK, /* message sent when fade has completed to black, param is the original caller */
	MSG_FADE_NONE, /* sent when fade has completed to none */
	MSG_FADE_START_NONE, /* sent when each state has started to change */
	MSG_FADE_START_BLACK,
};
