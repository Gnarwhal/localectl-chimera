#ifndef __LOCALECTL_COMMAND_H__
#define __LOCALECTL_COMMAND_H__

#include <stdbool.h>

#include "locale1.h"

#define COMMAND_STATUS                   0
#define COMMAND_SET_LOCALE               1
#define COMMAND_LIST_LOCALES             2
#define COMMAND_SET_KEYMAP               3
#define COMMAND_LIST_KEYMAPS             4
#define COMMAND_SET_X11_KEYMAP           5
#define COMMAND_LIST_X11_KEYMAP_MODELS   6
#define COMMAND_LIST_X11_KEYMAP_LAYOUTS  7
#define COMMAND_LIST_X11_KEYMAP_VARIANTS 8
#define COMMAND_LIST_X11_KEYMAP_OPTIONS  9
#define COMMAND_HELP                     10
#define COMMAND_VERSION                  11

// struct CommandStatus {}; -- needs no supplemental data

struct CommandSetLocale {
	char **locales;
	bool ask_password;
};

// struct Help    {}; -- needs no supplemental data
// struct Version {}; -- needs no supplemental data

struct Command {
	int type;
	union {
		// struct CommandStatus  status;     -- needs no supplemental data
		struct CommandSetLocale  set_locale;
		// struct CommandHelp    help;       -- needs no supplemental data
		// struct CommandVersion version;    -- needs no supplemental data
	};
	char error[64];
};

void command_status(LocalectlLocale1 *proxy);
void command_set_locale(LocalectlLocale1 *proxy, struct CommandSetLocale *set_locale);
void command_help(void);
void command_version(void);

#endif // __LOCALCTL_COMMAND_H__
