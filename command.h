#ifndef LOCALECTL_COMMAND
#define LOCALECTL_COMMAND

#define COMMAND_STATUS                   0
#define COMMAND_SET_LOCALE               1
#define COMMAND_LIST_LOCALES             2
#define COMMAND_SET_KEYMAP               3
#define COMMAND_LIST_KEYMAPS             4
#define COMMAND_SET_X11_KEYMAPS          5
#define COMMAND_LIST_X11_KEYMAP_MODELS   6
#define COMMAND_LIST_X11_KEYMAP_LAYOUTS  7
#define COMMAND_LIST_X11_KEYMAP_VARIANTS 8
#define COMMAND_LIST_X11_KEYMAP_OPTIONS  9
#define COMMAND_HELP                     10
#define COMMAND_VERSION                  11

// struct Status {}; -- needs no supplemental data

struct CommandSetLocale {
	char * lang;
	char * lc_ctype;
	char * lc_numeric;
	char * lc_time;
	char * lc_collate;
	char * lc_monetary;
	char * lc_messages;
	char * lc_all;
};

// struct Help    {}; -- needs no supplemental data
// struct Version {}; -- needs no supplemental data

struct Command {
	int type;
	union {
		// struct Status           status; -- needs no supplemental data
		struct CommandSetLocale    set_locale;
		// struct Help             help; -- needs no supplemental data
		// struct Version          version; -- needs no supplemental data
		const char * error;
	};
};

void command_status(void);
void command_help(void);
void command_version(void);

#endif // LOCALCTL_COMMAND
