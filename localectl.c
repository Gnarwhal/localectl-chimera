#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "ansi_codes.h"
#include "command.h"
#include "gio/gio.h"
#include "locale1.h"

#define PROJECT_NAME "localectl"

static const int ANY_AMOUNT_ARG = -1;

bool verify_arg_count(int argc, int min_argc, int max_argc) {
	if (argc < min_argc) {
		fprintf(stderr, ANSI_ESCAPE(ANSI_RED";"ANSI_HIGHLIGHT) "Too few arguments." ANSI_ESCAPE(ANSI_RESET) "\n");
		return false;
	} else if (argc > max_argc && max_argc != ANY_AMOUNT_ARG) {
		fprintf(stderr, ANSI_ESCAPE(ANSI_RED";"ANSI_HIGHLIGHT) "Too many arguments." ANSI_ESCAPE(ANSI_RESET) "\n");
		return false;
	}

	return true;
}

int main(int argc, char **argv) {
	/* --- Option parsing courtesy of systemd-localectl --- */
	enum {
		ARG_VERSION,
		ARG_NO_PAGER,
		ARG_NO_CONVERT,
		ARG_NO_ASK_PASSWORD
	};
	static const struct option options[] = {
		{ "help",            no_argument,       NULL, 'h'                 },
		{ "version",         no_argument,       NULL, ARG_VERSION         },
		{ "no-pager",        no_argument,       NULL, ARG_NO_PAGER        },
		{ "host",            required_argument, NULL, 'H'                 },
		{ "machine",         required_argument, NULL, 'M'                 },
		{ "no-ask-password", no_argument,       NULL, ARG_NO_ASK_PASSWORD },
		{ "no-convert",      no_argument,       NULL, ARG_NO_CONVERT      },
	};

	int c;
	bool arg_ask_password = true;

	assert(argc >= 0);
	assert(argv);

	while ((c = getopt_long(argc, argv, "h" /*"H:" "M:"*/, options, NULL)) >= 0) {
		switch (c) {
			case 'h':
				command_help();
				return 0;

			case ARG_VERSION:
				command_version();
				return 0;

			case ARG_NO_CONVERT:
				// arg_convert = false;
				break;

			case ARG_NO_PAGER:
				// arg_pager_flags |= PAGER_DISABLE;
				break;

			case ARG_NO_ASK_PASSWORD:
				arg_ask_password = false;
				break;

			case 'H':
				// arg_transport = BUS_TRANSPORT_REMOTE;
				// arg_host = optarg;
				break;

			case 'M':
				// arg_transport = BUS_TRANSPORT_MACHINE;
				// arg_host = optarg;
				break;

			default:
				fprintf(stderr, "localectl: invalid option -- '%c'\n", c);
				return 1;
		}
	}

	/* --- DBus Proxy Variables --- */
	LocalectlLocale1 *proxy = NULL;
	GError *error = NULL;

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

	argc -= optind + 1;
	argv += optind;
	optind = 0;

	const char *command_name = argv[0];

	       if (strcmp("status",       command_name) == 0) { if (!verify_arg_count(argc, 0, 0             )) { return 1; } command_status(proxy);
	} else if (strcmp("set-locale",   command_name) == 0) { if (!verify_arg_count(argc, 1, ANY_AMOUNT_ARG)) { return 1; } command_set_locale(proxy, argv, arg_ask_password);
	} else if (strcmp("list-locales", command_name) == 0) { if (!verify_arg_count(argc, 0, 0             )) { return 1; } command_list_locales();
	} else if (strcmp("list-keymaps", command_name) == 0) { if (!verify_arg_count(argc, 0, 0             )) { return 1; } command_list_keymaps();
	} else {
		fprintf(stderr, ANSI_ESCAPE(ANSI_RED";"ANSI_HIGHLIGHT) "Unknown operation %s." ANSI_ESCAPE(ANSI_RESET) "\n", command_name);
		return 1;
	}

	if (proxy) {
		g_object_unref(proxy);
	}

	return 0;
}
