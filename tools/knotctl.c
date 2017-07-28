/*
 * Copyright (c) 2017, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <glib.h>

static gboolean opt_set_user = FALSE;
static char *opt_uuid;
static char *opt_token;

static int cmd_set_user(void)
{
	if (!opt_uuid) {
		printf("User's UUID missing!\n");
		return -EINVAL;
	}

	if (!opt_token) {
		printf("User's TOKEN missing!\n");
		return -EINVAL;
	}

	printf("Saving user credentials: %s %s...\n", opt_uuid, opt_token);

	return 0;
}

static GOptionEntry options[] = {
	{ "uuid", 'u', 0, G_OPTION_ARG_STRING, &opt_uuid,
						"User's UUID", "UUID" },
	{ "token", 't', 0, G_OPTION_ARG_STRING, &opt_token,
						"User's token", "TOKEN" },
	{ NULL },
};

static GOptionEntry commands[] = {
	{ "set-user", 0, 0, G_OPTION_ARG_NONE, &opt_set_user,
	"Set user's credentials. " \
	"Eg: ./knotctl --set-user -u value -t value", NULL},
	{ NULL },
};

int main(int argc, char *argv[])
{
	GOptionContext *context;
	GOptionGroup *opt_group;
	GError *gerr = NULL;
	int err;

	opt_uuid = NULL;
	opt_token = NULL;

	printf("KNOT Control CLI tool\n");

	context = g_option_context_new(NULL);
	g_option_context_add_main_entries(context, commands, NULL);

	opt_group = g_option_group_new("options", "Options usage",
					"Show all set options", NULL, NULL);
	g_option_context_add_group(context, opt_group);
	g_option_group_add_entries(opt_group, options);

	if (!g_option_context_parse(context, &argc, &argv, &gerr)) {
		printf("Invalid arguments: %s\n", gerr->message);
		g_error_free(gerr);
		g_option_context_free(context);
		exit(EXIT_FAILURE);
	}

	g_option_context_free(context);

	if (opt_set_user)
		err = cmd_set_user();

	printf("Exiting\n");

	return err;
}
