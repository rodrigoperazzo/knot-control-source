/*
 * Copyright (c) 2017, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gio/gio.h>

static guint dbus_id;
static GMainLoop *main_loop;

static struct user {
	gchar *uuid;
	gchar *token;
} user;

static GDBusNodeInfo *introspection_data;

/* Introspection data for the service we are exporting */
static const gchar introspection_xml[] =
	"<node>"
	"  <interface name='org.cesar.knot.control.Manager'>"
	"    <method name='SetUser'>"
	"      <arg type='s' name='uuid' direction='in'/>"
	"      <arg type='s' name='token' direction='in'/>"
	"      <arg type='b' name='response' direction='out'/>"
	"    </method>"
	"    <property type='s' name='UserUUID' access='read'/>"
	"    <property type='s' name='UserTOKEN' access='read'/>"
	"  </interface>"
	"</node>";

static void handle_method_call(GDBusConnection *connection,
					const gchar *sender,
					const gchar *object_path,
					const gchar *interface_name,
					const gchar *method_name,
					GVariant *parameters,
					GDBusMethodInvocation *invocation,
					gpointer user_data)
{
	const gchar *uuid;
	const gchar *token;
	gboolean response;

	if (g_strcmp0(method_name, "SetUser") == 0) {
		g_variant_get(parameters, "(&s&s)", &uuid, &token);
		g_free(user.uuid);
		g_free(user.token);
		user.uuid = g_strdup(uuid);
		user.token = g_strdup(token);
		printf("uuid %s token %s", user.uuid, user.token);

		response = TRUE;
		g_dbus_method_invocation_return_value(invocation,
				g_variant_new("(b)", response));
	}
}

static GVariant *handle_get_property(GDBusConnection  *connection,
					const gchar *sender,
					const gchar *object_path,
					const gchar *interface_name,
					const gchar *property_name,
					GError **gerr,
					gpointer user_data)
{
	GVariant *gvar = NULL;

	if (g_strcmp0(property_name, "UserUUID") == 0)
		gvar = g_variant_new("s", user.uuid);
	else if (g_strcmp0(property_name, "UserTOKEN") == 0)
		gvar = g_variant_new("s", user.token);

	return gvar;
}

static const GDBusInterfaceVTable interface_vtable = {
	handle_method_call,
	handle_get_property,
	NULL
};

static void on_bus_acquired(GDBusConnection *connection, const gchar *name,
							gpointer user_data)
{
	guint registration_id;

	registration_id = g_dbus_connection_register_object(connection,
					"/org/cesar/knot/control",
					introspection_data->interfaces[0],
					&interface_vtable,
					NULL,  /* user_data */
					NULL,  /* user_data_free_func */
					NULL); /* GError** */

	g_assert(registration_id > 0);
}


static void on_name_acquired(GDBusConnection *connection, const gchar *name,
							gpointer user_data)
{
	/* Connection successfully estabilished */
	printf("Connection estabilished\n");
}

static void on_name_lost(GDBusConnection *connection, const gchar *name,
							gpointer user_data)
{
	if (!connection) {
		/* Connection error */
		printf("Connection failure\n");
	} else {
		/* Name not owned */
		printf("Name can't be obtained\n");
	}

	g_free(user.uuid);
	g_free(user.token);
	g_dbus_node_info_unref(introspection_data);
	exit(EXIT_FAILURE);
}

static guint dbus_init(void)
{
	guint owner_id;

	introspection_data = g_dbus_node_info_new_for_xml(introspection_xml, NULL);
	g_assert(introspection_data != NULL);

	owner_id = g_bus_own_name(G_BUS_TYPE_SYSTEM,
					"org.cesar.knot.control",
					G_BUS_NAME_OWNER_FLAGS_NONE,
					on_bus_acquired, on_name_acquired,
					on_name_lost, NULL, NULL);
	return owner_id;
}

static void dbus_on_close(guint owner_id)
{
	g_free(user.uuid);
	g_free(user.token);
	g_bus_unown_name(owner_id);
	g_dbus_node_info_unref(introspection_data);
}

static void sig_term(int sig)
{
	g_main_loop_quit(main_loop);
}

int main(int argc, char *argv[])
{
	printf("KNOT Control daemon\n");

	signal(SIGTERM, sig_term);
	signal(SIGINT, sig_term);
	signal(SIGPIPE, SIG_IGN);

	memset(&user, 0, sizeof(struct user));

	main_loop = g_main_loop_new(NULL, FALSE);

	dbus_id = dbus_init();

	g_main_loop_run(main_loop);

	dbus_on_close(dbus_id);

	g_main_loop_unref(main_loop);

	return 0;
}
