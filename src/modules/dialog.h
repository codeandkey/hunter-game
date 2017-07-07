#pragma once

#include <tds/tds.h>

/*
 * Sequence file format:
 *
 * @sequence_name
 * :stringdb_index:stringdb_offset:portrait_texture_index
 * ...
 */

#define MOD_DIALOG_PREFIX "res/seq/"

#define MOD_DIALOG_RENDER_TOP_LEFT 0
#define MOD_DIALOG_RENDER_TOP_RIGHT 1
#define MOD_DIALOG_RENDER_BOTTOM_LEFT 2
#define MOD_DIALOG_RENDER_BOTTOM_RIGHT 3

#define MOD_DIALOG_SEQ_NAMELEN 32
#define MOD_DIALOG_FILE_BUFLEN 256

#define MOD_DIALOG_CHAR_INTERVAL 30

#define MOD_DIALOG_SIZE_PX 100
#define MOD_DIALOG_TEXT_PADDING_PX 20
#define MOD_DIALOG_EXTRA_PADDING 250

#define MOD_DIALOG_PORTRAIT_FRAME "res/sprites/portrait_frame_128x128.png"
#define MOD_DIALOG_FONT           "dialog"
#define MOD_DIALOG_SMALLFONT      "dialogheader"
#define MOD_DIALOG_SEQFILE        "res/seq/debug"

struct mod_dialog_seq_entry {
	struct tds_string* dialog_string;
	struct tds_string* header_string;
	struct tds_texture* texture_portrait;
	struct mod_dialog_seq_entry* next;
};

struct mod_dialog_sequence {
	char name[MOD_DIALOG_SEQ_NAMELEN];
	struct mod_dialog_sequence* next;
	struct mod_dialog_seq_entry* head, *tail;
};

struct mod_dialog {
	struct mod_dialog_sequence* sequences, *cur_sequence;
	struct mod_dialog_seq_entry* cur_entry; /* Set when a sequence starts. */
	struct tds_texture* portrait_frame;
	struct tds_font* font, *font_small;
	int cur_entry_pos; /* Current position of the target string. */
	tds_clock_point cp; /* Used for all sorts of things. */
};

void mod_dialog_init(void* data);
void mod_dialog_destroy(void* data);
void mod_dialog_draw(void* data);
void mod_dialog_msg(void* data, int msg, void* param);

struct mod_dialog* mod_dialog_create(const char* seq_filename, struct tds_texture* portrait_frame, struct tds_font* font);
void mod_dialog_free(struct mod_dialog* ptr);

void mod_dialog_start_sequence(struct mod_dialog* ptr, char* name);
int mod_dialog_get_active(struct mod_dialog* ptr);

/* Any dialog updating keypress will have the same action -- skip to the end if the cursor is not at the end and go to the next sequence if it is. */
void mod_dialog_send_keypress(struct mod_dialog* ptr);

void mod_dialog_render(struct mod_dialog* ptr);

extern struct tds_module_template mod_dialog_type;
