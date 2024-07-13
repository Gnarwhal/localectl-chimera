#ifndef __LOCALECTL_COMMAND_H__
#define __LOCALECTL_COMMAND_H__

#include <stdbool.h>

#include "locale1.h"

void command_status(LocalectlLocale1 *proxy);
void command_set_locale(LocalectlLocale1 *proxy, char **locales, bool ask_password);
void command_list_locales(void);
void command_set_keymap(LocalectlLocale1 *proxy, const char *keymap, /* bool convert, */ bool ask_password);
void command_list_keymaps(void);
void command_set_x11_keymap(LocalectlLocale1 *proxy, const char *layout, const char *model, const char *variant, const char *options, /* bool convert, */ bool ask_password);
void command_list_x11_keymap_models(void);
void command_list_x11_keymap_layouts(void);
void command_list_x11_keymap_variants(const char *layout);
void command_list_x11_keymap_options(void);
void command_help(void);
void command_version(void);

#endif // __LOCALCTL_COMMAND_H__
