#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ansi_codes.h"
#include "locale1.h"
#include "version.h"

#include "command.h"

bool postfix_is(const char *str, const char *postfix) {
	int str_length    = strlen(str);
	int postfix_length = strlen(postfix);

	if (str_length < postfix_length) {
		return false;
	}

	return strcmp(str + str_length - postfix_length, postfix) == 0;
}

int for_directory(const char *dir_path, bool recurse, void (*callback)(const char *file_path)) {
	DIR *dir = opendir(dir_path);
	if (!dir) {
		fprintf(stderr, "Failed to open dir: %s\n", dir_path);
		return 1;
	}

	struct dirent *dir_entry;
	while ((dir_entry = readdir(dir))) {
		if (recurse && strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..")) {
			// A directory path would never be longer than 512 characters :)
			char subdir_path[512];
			snprintf(subdir_path, 512, "%s/%s", dir_path, dir_entry->d_name);
			struct stat path_stat;
			if (stat(subdir_path, &path_stat) != 0) {
				fprintf(stderr, "Failed to read directory entry: %s\n", subdir_path);
			} else if (S_ISDIR(path_stat.st_mode)) {
				for_directory(subdir_path, true, callback);
			} else {
				callback(dir_entry->d_name);
			}
		} else {
			callback(dir_entry->d_name);
		}
	}
	
	closedir(dir);
	return 0;
}

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

	// Please don't try and `localectl set-locale haha=yourprogramisrubbish` and then tell me as such when it doesn't like that. I'm already aware ;-;
	if (locales[1] == NULL && strstr(locales[0], "=") == NULL) {
		snprintf(lang, 32, "LANG=%s", locales[0]);
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
			fprintf(stderr, "Error setting locale | %s\n", error->message);
		}
		g_object_unref(proxy);
		g_error_free(error);
		exit(1);
	}
}

// Instead of reading /usr/share/locale, just print the locales included in musl-locale
// But just in case we think otherwise in the future...
//
// void print_locale(const char *file_name) {
// 	/* --- systemd filters out non utf8 locales. As such, so shall we --- */
// 	if (postfix_is(file_name, ".UTF-8")) {
// 		printf("%s\n", file_name);
// 	}
// }

// void command_list_locales(void) {
// 	static const char *LOCALE_DIRECTORY = "/usr/share/locale";
// 	print_locale("C.UTF-8");
// 	for_directory(LOCALE_DIRECTORY, false, print_locale);
// }

void command_list_locales(void) {
	// I hope I got all the inbuilt locales ¯\_(ツ)_/¯
	printf(
		"C\n"
		"C.UTF-8\n"
		"cs_CZ\n"
		"de_DE\n"
		"en_GB\n"
		"es_ES\n"
		"fi_FI\n"
		"fr_FR\n"
		"it_IT\n"
		"nb_NO\n"
		"nl_NL\n"
		"pt_BR\n"
		"pt_PT\n"
		"ru_RU\n"
		"sr_RS\n"
		"sv_SE\n"
	);
}

void command_set_keymap(LocalectlLocale1 *proxy, const char *keymap, const char *toggle, /* bool convert, */ bool ask_password) {
	GError *error = NULL;
	
	localectl_locale1_call_set_vconsole_keyboard_sync(
		proxy,
		keymap,
		toggle ? toggle : "",
		//convert,
		false,
		ask_password,
		NULL,
		&error
	);
	

	if (error) {
		fprintf(stderr, "Error setting keymap | %s\n", error->message);
		g_object_unref(proxy);
		g_error_free(error);
		exit(1);
	}
}

void print_keymap(const char *file_name) {
	char stripped_file_name[128];
	if (postfix_is(file_name, ".map")) {
		snprintf(stripped_file_name, MIN(strlen(file_name) - 3, 128), "%s", file_name);
		printf("%s\n", stripped_file_name);
	} else if (postfix_is(file_name, ".map.gz")) {
		snprintf(stripped_file_name, MIN(strlen(file_name) - 6, 128), "%s", file_name);
		printf("%s\n", stripped_file_name);
	}
}

void command_list_keymaps(void) {
	static const char *KEYMAP_DIRECTORY = "/usr/share/keymaps";
	for_directory(KEYMAP_DIRECTORY, true, print_keymap);
}

void command_set_x11_keymap(LocalectlLocale1 *proxy, const char *layout, const char *model, const char *variant, const char *options, /* bool convert, */ bool ask_password) {
	GError *error = NULL;

	localectl_locale1_call_set_x11_keyboard_sync(
		proxy,
		layout,
		model,
		variant,
		options,
		//convert,
		false,
		ask_password,
		NULL,
		&error
	);
	

	if (error) {
		fprintf(stderr, "Error setting x11 keymap | %s\n", error->message);
		g_object_unref(proxy);
		g_error_free(error);
		exit(1);
	}
}

bool is_empty(const char *str) {
	while (*str) {
		if (!isspace(*str)) {
			return false;
		}
		++str;
	}
	return true;
}

void list_x11(const char *type, const char *layout) {
	static const char *X11_KEYMAP_BASE_LIST = "/usr/share/X11/xkb/rules/base.lst";

	FILE *list = fopen(X11_KEYMAP_BASE_LIST, "r");

	if (!list) {
		fprintf(stderr, "Error reading x11 base file list");
	}

	char filter[32];
	if (layout != NULL) {
		snprintf(filter, 32, " %s:", layout);
	}

	char line[128];
	bool in_correct_section = false;
	while (fgets(line, 128, list)) {
		if (!is_empty(line)) {
			if (line[0] == '!') {
				in_correct_section = strncmp(line + 2, type, strlen(type)) == 0;
			} else if (in_correct_section) {
				if (!layout || strstr(line + 2, filter)) {
					printf("%s", line + 2);
				}
			}
		}
	}
}

void command_list_x11_keymap_models(void) {
	list_x11("model", NULL);
}

void command_list_x11_keymap_layouts(void) {
	list_x11("layout", NULL);
}

void command_list_x11_keymap_variants(const char *layout) {
	list_x11("variant", layout);
}

void command_list_x11_keymap_options(void) {
	list_x11("option", NULL);
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
