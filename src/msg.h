#pragma once

enum hunter_msg {
	MSG_CAMERA_TRACK,
	MSG_CAMERA_SHAKE,
	MSG_CAMERA_SIZE,
	MSG_PLAYER_HIT,
	MSG_PLAYER_ACTION,
	MSG_SAVESTATION_START,
	MSG_SAVESTATION_END,
	MSG_SAVESTATION_QUERY,
	MSG_WORLD_READY,
	MSG_BGM_REQ_START,
	MSG_BGM_REQ_STOP,
	MSG_BGM_START, /* broadcasted FROM bgm when bgm is actually started -- do NOT send */
	MSG_BGM_STOP, /* ^ for stopped */
	MSG_DIALOG_REQ_START,
	MSG_DIALOG_REQ_STOP,
	MSG_DIALOG_KP, /* broadcast whenever you want dialog to advance */
	MSG_DIALOG_START, /* don't send these.. only dialog will */
	MSG_DIALOG_STOP,
};
