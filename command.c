#include <stdio.h>

#include "ansi_codes.h"
#include "glib.h"
#include "version.h"

#include "command.h"

void command_status(LocalectlLocale1 *proxy) {
	const char *const *locale = localectl_locale1_get_locale(proxy);
	const char *status_lang     = locale[0];
	const char *status_language = locale[1];

	const char *status[] = {
		status_lang,
		status_language,
		localectl_locale1_get_vconsole_keymap(proxy),
		localectl_locale1_get_vconsole_keymap_toggle(proxy),
		localectl_locale1_get_x11_layout(proxy),
		localectl_locale1_get_x11_model(proxy),
		localectl_locale1_get_x11_variant(proxy)
	};

	static const int STATUS_LENGTH = 7;
	for (int i = 0; i < STATUS_LENGTH; ++i) {
		if (status[i] == NULL) {
			switch (i) {
				case  0: status[i] = "LANG=";     break;
				case  1: status[i] = "LANGUAGE="; break;
				default: status[i] = "";          break;
			}
		}
	}
	
	printf(
		"   System Locale: %s\n"
		"                  %s\n"
		"       VC Keymap: %s\n",
		status[0],
		status[1],
		status[2]
	);
	if (strcmp(status[3], "") != 0) {
		printf("VC Toggle Keymap: %s\n", status[3]);
	}
	printf(
		"      X11 Layout: %s\n"
		"       X11 Model: %s\n"
		"     X11 Variant: %s\n",
		status[4],
		status[5],
		status[6]
	);
}

void command_set_locale(LocalectlLocale1 *proxy, char **locales, bool ask_password) {
	char lang[32];
	GError *error = NULL;

	// If only given a single arg and it isn't already 'LANG=??', set it to LANG=${arg}
	if (locales[1] == NULL && strncmp("LANG=", locales[0], strlen("LANG="))) {
		sprintf(lang, "LANG=%s", locales[0]);
		locales[0] = lang;
	}
	localectl_locale1_call_set_locale_sync(
		proxy,
		locales,
		ask_password,
		NULL,
		&error
	);
	

	if (error) {
		if (error->code == 16) {
			fprintf(stderr, ANSI_ESCAPE(ANSI_RED";"ANSI_HIGHLIGHT)"Failed to issue method call: Invalid Locale data."ANSI_ESCAPE(ANSI_RESET)"\n");
		} else {
			// Not sure what other errors it can give, but they're handled if they happen
			fprintf(stderr, "Error setting locale: %s\n", error->message);
		}
		g_object_unref(proxy);
		g_error_free(error);
		exit(1);
	}
}

void command_help(void) {
	/* --- Help message courtesy of systemd-localectl --- */
	printf("localectl [OPTIONS...] COMMAND ...\n"
	       "\n"
	       ANSI_ESCAPE(ANSI_HIGHLIGHT) "Query or change system locale and keyboard settings." ANSI_ESCAPE(ANSI_RESET)"\n"
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
	       // "     --no-pager            Do not pipe output into a pager\n"
	       "     --no-ask-password     Do not prompt for password\n"
	       // "  -H --host=[USER@]HOST    Operate on remote host\n"
	       // "  -M --machine=CONTAINER   Operate on local container\n"
	       // "     --no-convert          Don't convert keyboard mappings\n"
		   "\n"
	);
}

void command_version(void) {
	printf("localectl (Chimera) version "VERSION_STR"\n");
}
