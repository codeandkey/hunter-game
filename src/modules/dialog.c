#include "dialog.h"
#include "../msg.h"

struct tds_module_template mod_dialog_type = {
	.name = "dialog",
	.data_size = sizeof(struct mod_dialog),
	.init = mod_dialog_init,
	.destroy = mod_dialog_destroy,
	.update = NULL,
	.draw = mod_dialog_draw,
	.msg = mod_dialog_msg,
};

void mod_dialog_init(void* data) {
	struct mod_dialog* ptr = (struct mod_dialog*) data;

	ptr->sequences = NULL;
	ptr->cur_entry = NULL;
	ptr->portrait_frame = tds_texture_cache_get(tds_engine_global->tc_handle, MOD_DIALOG_PORTRAIT_FRAME, -1, -1, 0, 0);

	ptr->cp = tds_clock_get_point();
	ptr->cur_entry_pos = 0;
	ptr->font = tds_font_cache_get(tds_engine_global->fc_handle, MOD_DIALOG_FONT);

	/* Now, we load in the sequences file. */

	char buf[MOD_DIALOG_FILE_BUFLEN] = {0};
	FILE* fd = fopen(MOD_DIALOG_SEQFILE, "r");

	if (!fd) {
		tds_logf(TDS_LOG_CRITICAL, "Failed to open sequence file [%s] for reading\n", MOD_DIALOG_SEQFILE);
		return;
	}

	struct mod_dialog_sequence* cur_sequence = NULL;

	while (fgets(buf, sizeof buf / sizeof *buf, fd)) {
		buf[strcspn(buf, "\n")] = 0;

		if (buf[0] == '@') {
			/* Sequence name incoming! */
			cur_sequence = tds_malloc(sizeof *cur_sequence);

			int buflen = strlen(buf + 1), cpylen = buflen;
			if (buflen >= MOD_DIALOG_SEQ_NAMELEN) {
				cpylen = MOD_DIALOG_SEQ_NAMELEN - 1;
			}

			memcpy(cur_sequence->name, buf + 1, cpylen);
			cur_sequence->name[MOD_DIALOG_SEQ_NAMELEN - 1] = 0;

			cur_sequence->next = ptr->sequences;
			cur_sequence->head = cur_sequence->tail = NULL;
			ptr->sequences = cur_sequence;
		}

		if (buf[0] == ':') {
			if (!cur_sequence) {
				tds_logf(TDS_LOG_WARNING, "No active sequence near [%s], ignoring entry!\n", buf);
				continue;
			}

			struct mod_dialog_seq_entry* new_entry = tds_malloc(sizeof *new_entry);

			char* stringdb_index = strtok(buf + 1, ":"), *stringdb_offset = strtok(NULL, ":"), *texture_name = strtok(NULL, ":"), *saveptr = NULL;

			if (!stringdb_index || !stringdb_offset || !texture_name) {
				tds_logf(TDS_LOG_WARNING, "Invalid format near [%s], ignoring entry\n", buf);
				continue;
			}

			int stringdb_offset_val = strtol(stringdb_offset, &saveptr, 10);
			new_entry->dialog_string = tds_stringdb_get(tds_engine_global->stringdb_handle, stringdb_index, strlen(stringdb_index), stringdb_offset_val);
			new_entry->texture_portrait = tds_texture_cache_get(tds_engine_global->tc_handle, texture_name, -1, -1, 0, 0);
			new_entry->next = NULL;

			if (!cur_sequence->head) {
				cur_sequence->head = new_entry;
			}

			if (cur_sequence->tail) {
				cur_sequence->tail->next = new_entry;
			}

			cur_sequence->tail = new_entry;
		}
	}
}

void mod_dialog_destroy(void* data) {
	struct mod_dialog* ptr = (struct mod_dialog*) data;
	struct mod_dialog_sequence* cur_sequence = ptr->sequences, *tmp_sequence = NULL;

	while (cur_sequence) {
		struct mod_dialog_seq_entry* cur_entry = cur_sequence->head, *tmp_entry = NULL;

		while(cur_entry) {
			tmp_entry = cur_entry->next;
			tds_free(cur_entry);
			cur_entry = tmp_entry;
		}

		tmp_sequence = cur_sequence->next;
		tds_free(cur_sequence);
		cur_sequence = tmp_sequence;
	}
}

void mod_dialog_start_sequence(struct mod_dialog* ptr, char* name) {
	int name_len = strlen(name);

	if (name_len >= MOD_DIALOG_SEQ_NAMELEN) {
		return;
	}

	struct mod_dialog_sequence* cur_sequence = ptr->sequences;

	while (cur_sequence) {
		if (!strncmp(cur_sequence->name, name, name_len)) {
			/* Name matches, prepare this sequence. */
			ptr->cur_entry = cur_sequence->head;
			ptr->cur_sequence = cur_sequence;
			ptr->cur_entry_pos = 0;
			ptr->cp = tds_clock_get_point();
			tds_engine_broadcast(tds_engine_global, MSG_DIALOG_START, ptr->cur_sequence->name);
			return;
		}

		cur_sequence = cur_sequence->next;
	}

	tds_logf(TDS_LOG_WARNING, "Did not find sequence name [%s]\n", name);
}

void mod_dialog_send_keypress(struct mod_dialog* ptr) {
	if (!ptr->cur_entry) {
		return;
	}

	if (ptr->cur_entry_pos == ptr->cur_entry->dialog_string->len) {
		ptr->cur_entry = ptr->cur_entry->next;

		if (!ptr->cur_entry) {
			tds_engine_broadcast(tds_engine_global, MSG_DIALOG_STOP, ptr->cur_sequence->name);
		}
	} else {
		ptr->cur_entry_pos = ptr->cur_entry->dialog_string->len;
	}
}

void mod_dialog_draw(void* data) {
	struct mod_dialog* ptr = (struct mod_dialog*) data;

	if (!ptr->cur_entry) {
		return;
	}

	int next_pos = tds_clock_get_ms(ptr->cp) / MOD_DIALOG_CHAR_INTERVAL;

	if (next_pos > ptr->cur_entry_pos) {
		ptr->cur_entry_pos = next_pos;
	}

	if (ptr->cur_entry_pos > ptr->cur_entry->dialog_string->len) {
		ptr->cur_entry_pos = ptr->cur_entry->dialog_string->len;
	}

	tds_render_flat_set_mode(tds_engine_global->render_flat_overlay_handle, TDS_RENDER_COORD_REL_SCREENSPACE);
	tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 1.0f, 1.0f, 1.0f, 1.0f);
	tds_render_flat_text(tds_engine_global->render_flat_overlay_handle, ptr->font, ptr->cur_entry->dialog_string->data, ptr->cur_entry_pos, -0.9f, 0.9f, TDS_RENDER_LALIGN, ptr->cur_entry->dialog_string->formats);

	tds_logf(TDS_LOG_DEBUG, "rendering dialog text [%.*s]\n", ptr->cur_entry_pos, ptr->cur_entry->dialog_string->data);
}

void mod_dialog_msg(void* data, int msg, void* param) {
	switch (msg) {
	case MSG_DIALOG_KP:
		mod_dialog_send_keypress((struct mod_dialog*) data);
		break;
	case MSG_DIALOG_REQ_START:
		break;
	case MSG_DIALOG_REQ_STOP:
		break;
	}
}
