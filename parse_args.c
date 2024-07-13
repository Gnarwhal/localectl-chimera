#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "ansi_codes.h"

#include "parse_args.h"

static const int ANY_AMOUNT_ARG = -1;

bool verify_arg_count(int argc, int min_argc, int max_argc, char *error) {
	if (argc < min_argc) {
		sprintf(error, ANSI_ESCAPE(ANSI_RED";"ANSI_HIGHLIGHT) "Too few arguments." ANSI_ESCAPE(ANSI_RESET));
		return false;
	} else if (argc > max_argc && max_argc != ANY_AMOUNT_ARG) {
		sprintf(error, ANSI_ESCAPE(ANSI_RED";"ANSI_HIGHLIGHT) "Too many arguments." ANSI_ESCAPE(ANSI_RESET));
		return false;
	}

	return true;
}

int parse_args(int argc, char **argv, struct Command *command) {
	assert(command != NULL);

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
				command->type = COMMAND_HELP;
				return 0;

			case ARG_VERSION:
				command->type = COMMAND_VERSION;
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
				sprintf(command->error, "localectl: invalid option -- '%c'", c);
				return 1;
		}
	}

	// If no command specified, default to status
	if (argc == 1) {
		command->type = COMMAND_STATUS;
	}

	argc -= optind + 1;
	argv += optind;
	optind = 0;

	const char *command_name = argv[0];

	if (strcmp("status", command_name) == 0) {
		if (!verify_arg_count(argc, 0, 0,              command->error)) { return 1; }
		command->type = COMMAND_STATUS;
	} else if (strcmp("set-locale", command_name) == 0) {
		if (!verify_arg_count(argc, 1, ANY_AMOUNT_ARG, command->error)) { return 1; }
		command->type                    = COMMAND_SET_LOCALE;
		command->set_locale.locales      = argv + 1;
		command->set_locale.ask_password = arg_ask_password;
	} else {
		sprintf(command->error, ANSI_ESCAPE(ANSI_RED";"ANSI_HIGHLIGHT) "Unknown operation %s" ANSI_ESCAPE(ANSI_RESET), command_name);
		return 1;
	}
	return 0;
}
