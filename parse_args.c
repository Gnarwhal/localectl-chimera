#include <assert.h>
#include <getopt.h>
#include <stddef.h>

#include "parse_args.h"

int parse_args(int argc, char **argv, struct Command *command) {
	assert(command != NULL);

	// If no command specified, default to status
	if (argc == 1) {
		command->type = COMMAND_STATUS;
	}

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

	assert(argc >= 0);
	assert(argv);

	while ((c = getopt_long(argc, argv, "hH:M:", options, NULL)) >= 0) {
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
				// arg_ask_password = false;
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
				return 1;
		}
	}

	return 0;
}
