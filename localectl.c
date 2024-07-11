#include <stdio.h>
#include "org.freedesktop.locale1.h"

#define PROJECT_NAME "localectl"

int main(int argc, char **argv) {
	/* --- Setup DBus Proxy --- */
	LocalectlLocale1 *proxy = NULL;
	GError *error = NULL;

	proxy = localectl_locale1_proxy_new_for_bus_sync(
		G_BUS_TYPE_SESSION,
		G_DBUS_PROXY_FLAGS_NONE,
		"org.freedesktop",
		"/org/freedesktop/locale1",
		NULL,
		&error
	);

	if (error) {
		fprintf(stderr, "%s", error->message);
		g_error_free(error);
		return 1;
	}

	g_object_unref(proxy);

	return 0;
}
