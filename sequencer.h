#ifndef SEQUENCER_H
#define SEQUENCER_H

const char *git_path_seq_dir(void);

#define APPEND_SIGNOFF_DEDUP (1u << 0)

enum replay_action {
	REPLAY_REVERT,
	REPLAY_PICK,
	REPLAY_INTERACTIVE_REBASE
};

struct replay_opts {
	enum replay_action action;

	/* Boolean options */
	int edit;
	int record_origin;
	int no_commit;
	int signoff;
	int allow_ff;
	int allow_rerere_auto;
	int allow_empty;
	int allow_empty_message;
	int keep_redundant_commits;
	int verbose;

	int mainline;

	const char *gpg_sign;

	/* Merge strategy */
	const char *strategy;
	const char **xopts;
	size_t xopts_nr, xopts_alloc;

	/* Only used by REPLAY_NONE */
	struct rev_info *revs;

	/* malloc()ed data entrusted to the sequencer */
	void **owned;
	int owned_nr, owned_alloc;
};
#define REPLAY_OPTS_INIT { -1 }

/*
 * Make it the duty of sequencer_remove_state() to release the memory;
 * For ease of use, return the same pointer.
 */
void *sequencer_entrust(struct replay_opts *opts, void *to_free);

int sequencer_pick_revisions(struct replay_opts *opts);
int sequencer_continue(struct replay_opts *opts);
int sequencer_rollback(struct replay_opts *opts);
int sequencer_remove_state(struct replay_opts *opts);

int sequencer_commit(const char *defmsg, struct replay_opts *opts,
			  int allow_empty, int edit, int amend,
			  int cleanup_commit_message);
int sequencer_make_script(int keep_empty, FILE *out,
		int argc, const char **argv);

int transform_todo_ids(int shorten_sha1s);
int check_todo_list(void);
int skip_unnecessary_picks(void);
int rearrange_squash(void);

extern const char sign_off_header[];

void append_signoff(struct strbuf *msgbuf, int ignore_footer, unsigned flag);
void append_conflicts_hint(struct strbuf *msgbuf);

#endif
