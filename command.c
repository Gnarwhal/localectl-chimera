#include <stdio.h>

#include "version.h"

#include "command.h"

void command_status(void) {}

void command_help(void) {
	/* --- Help message courtesy of systemd-localectl --- */
	printf("localectl [OPTIONS...] COMMAND ...\n"
	       "\n"
	       "Query or change system locale and keyboard settings.\n"
	       "\n"
		   "Commands:\n"
	       "  status                   Show current locale settings\n"
	       "  set-locale LOCALE...     Set system locale\n"
	       "  list-locales             Show known locales\n"
	       "  set-keymap MAP [MAP]     Set console and X11 keyboard mappings\n"
	       "  list-keymaps             Show known virtual console keyboard mappings\n"
	       "  set-x11-keymap LAYOUT [MODEL [VARIANT [OPTIONS]]]\n"
	       "                           Set X11 and console keyboard mappings\n"
	       "  list-x11-keymap-models   Show known X11 keyboard mapping models\n"
	       "  list-x11-keymap-layouts  Show known X11 keyboard mapping layouts\n"
	       "  list-x11-keymap-variants [LAYOUT]\n"
	       "                           Show known X11 keyboard mapping variants\n"
	       "  list-x11-keymap-options  Show known X11 keyboard mapping options\n"
	       "\nOptions:\n"
	       "  -h --help                Show this help\n"
	       "     --version             Show package version\n"
	       "     --no-pager            Do not pipe output into a pager\n"
	       "     --no-ask-password     Do not prompt for password\n"
	       "  -H --host=[USER@]HOST    Operate on remote host\n"
	       "  -M --machine=CONTAINER   Operate on local container\n"
	       "     --no-convert          Don't convert keyboard mappings\n"
		   "\n"
	);
}

void command_version(void) {
	printf("localectl (Chimera) version "VERSION_STR"\n");
}
