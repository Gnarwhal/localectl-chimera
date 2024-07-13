#include <stdio.h>

#include "command.h"
#include "gio/gio.h"
#include "locale1.h"
#include "parse_args.h"

#define PROJECT_NAME "localectl"

int main(int argc, char **argv) {
	/* --- Dispatch Command --- */
	struct Command command;
	if (parse_args(argc, argv, &command) != 0) {
		fprintf(stderr, "%s\n", command.error);
		return 1;
	}

	/* --- DBus Proxy Variables --- */
	LocalectlLocale1 *proxy = NULL;
	GError *error = NULL;

	/* --- Handle non-dbus commands --- */
	switch (command.type) {
		case COMMAND_HELP:    command_help();    break;
		case COMMAND_VERSION: command_version(); break;
		default: {
			/* --- Setup DBus Proxy --- */
			proxy = localectl_locale1_proxy_new_for_bus_sync(
				G_BUS_TYPE_SYSTEM,
				G_DBUS_PROXY_FLAGS_NONE,
				"org.freedesktop.locale1",
				"/org/freedesktop/locale1",
				NULL,
				&error
			);

			if (error) {
				fprintf(stderr, "Error creating proxy: %s\n", error->message);
				g_error_free(error);
				return 1;
			}
			break;
		}
	}

	/* --- Handle dbus commands --- */
	switch (command.type) {
		case COMMAND_STATUS:                   command_status(                  proxy                                   ); break;
		case COMMAND_SET_LOCALE:               command_set_locale(              proxy, &command.set_locale              ); break;
		// case COMMAND_LIST_LOCALES:             command_list_locales(            proxy, &command.list_locales            ); break;
		// case COMMAND_SET_KEYMAP:               command_set_keymap(              proxy, &command.set_keymap              ); break;
		// case COMMAND_LIST_KEYMAPS:             command_list_keymaps(            proxy, &command.list_keymaps            ); break;
		// case COMMAND_SET_X11_KEYMAP:           command_set_x11_keymap(          proxy, &command.set_x11_keymap          ); break;
		// case COMMAND_LIST_X11_KEYMAP_MODELS:   command_list_x11_keymap_models(  proxy, &command.list_x11_keymap_models  ); break;
		// case COMMAND_LIST_X11_KEYMAP_LAYOUTS:  command_list_x11_keymap_layouts( proxy, &command.list_x11_keymap_layouts ); break;
		// case COMMAND_LIST_X11_KEYMAP_VARIANTS: command_list_x11_keymap_variants(proxy, &command.list_x11_keymap_variants); break;
		// case COMMAND_LIST_X11_KEYMAP_OPTIONS:  command_list_x11_keymap_options( proxy, &command.list_x11_keymap_options ); break;
	}

	if (proxy) {
		g_object_unref(proxy);
	}

	return 0;
}
