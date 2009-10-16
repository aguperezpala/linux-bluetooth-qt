/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2005-2008  Marcel Holtmann <marcel@holtmann.org>
 *  Copyright (C) 2006-2007  Bastien Nocera <hadess@hadess.net>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include <dbus/dbus-glib.h>

#include <bluetooth-client.h>
#include <bluetooth-agent.h>

#include "notify.h"
#include "agent.h"

static BluetoothClient *client;
static GtkTreeModel *adapter_model;

typedef enum {
	AGENT_ERROR_REJECT
} AgentError;

#define AGENT_ERROR (agent_error_quark())

#define AGENT_ERROR_TYPE (agent_error_get_type()) 

static GQuark agent_error_quark(void)
{
	static GQuark quark = 0;
	if (!quark)
		quark = g_quark_from_static_string("agent");

	return quark;
}

#define ENUM_ENTRY(NAME, DESC) { NAME, "" #NAME "", DESC }

static GType agent_error_get_type(void)
{
	static GType etype = 0;
	if (etype == 0) {
		static const GEnumValue values[] = {
			ENUM_ENTRY(AGENT_ERROR_REJECT, "Rejected"),
			{ 0, 0, 0 }
		};

		etype = g_enum_register_static("agent", values);
	}

	return etype;
}

static GList *input_list = NULL;

struct input_data {
	char *path;
	char *uuid;
	gboolean numeric;
	DBusGProxy *device;
	DBusGMethodInvocation *context;
	GtkWidget *dialog;
	GtkWidget *button;
	GtkWidget *entry;
};

static gint input_compare(gconstpointer a, gconstpointer b)
{
	struct input_data *a_data = (struct input_data *) a;
	struct input_data *b_data = (struct input_data *) b;

	return g_ascii_strcasecmp(a_data->path, b_data->path);
}

static void input_free(struct input_data *input)
{
	gtk_widget_destroy(input->dialog);

	input_list = g_list_remove(input_list, input);

	if (input->device != NULL)
		g_object_unref(input->device);

	g_free(input->uuid);
	g_free(input->path);
	g_free(input);

	if (g_list_length(input_list) == 0)
		disable_blinking();
}


#ifndef DBUS_TYPE_G_DICTIONARY
#define DBUS_TYPE_G_DICTIONARY \
	(dbus_g_type_get_map("GHashTable", G_TYPE_STRING, G_TYPE_VALUE))
#endif

static gboolean device_get_properties(DBusGProxy *proxy,
					GHashTable **hash, GError **error)
{
	return dbus_g_proxy_call(proxy, "GetProperties", error,
		G_TYPE_INVALID, DBUS_TYPE_G_DICTIONARY, hash, G_TYPE_INVALID);
}

static gboolean pincode_request(DBusGMethodInvocation *context,
					DBusGProxy *device, gpointer user_data)
{
	GHashTable *hash = NULL;
	GValue *value;
	const gchar *address, *alias;
	gchar *name;

	device_get_properties(device, &hash, NULL);

	if (hash != NULL) {
		value = g_hash_table_lookup(hash, "Address");
		address = value ? g_value_get_string(value) : NULL;

		value = g_hash_table_lookup(hash, "Name");
		alias = value ? g_value_get_string(value) : NULL;
	} else {
		address = NULL;
		alias = NULL;
	}

	if (alias) {
		if (g_strrstr(alias, address))
			name = g_strdup(alias);
		else
			name = g_strdup_printf("%s (%s)", alias, address);
	} else
		name = g_strdup(address);

	printf ("nos llamaron a confirmar el pin \n");
	dbus_g_method_return(context, "0000");

	g_free(name);

	return TRUE;
}

static gboolean passkey_request(DBusGMethodInvocation *context,
					DBusGProxy *device, gpointer user_data)
{
	GHashTable *hash = NULL;
	GValue *value;
	const gchar *address, *alias;
	gchar *name;

	device_get_properties(device, &hash, NULL);

	if (hash != NULL) {
		value = g_hash_table_lookup(hash, "Address");
		address = value ? g_value_get_string(value) : NULL;

		value = g_hash_table_lookup(hash, "Name");
		alias = value ? g_value_get_string(value) : NULL;
	} else {
		address = NULL;
		alias = NULL;
	}

	if (alias) {
		if (g_strrstr(alias, address))
			name = g_strdup(alias);
		else
			name = g_strdup_printf("%s (%s)", alias, address);
	} else
		name = g_strdup(address);

	printf ("nos llamaron a confirmar el pin \n");
	dbus_g_method_return(context, "0000");

	g_free(name);

	return TRUE;
}


static gboolean adapter_insert(GtkTreeModel *model, GtkTreePath *path,
					GtkTreeIter *iter, gpointer user_data)
{
	BluetoothAgent *agent;
	DBusGProxy *adapter;

	gtk_tree_model_get(model, iter, BLUETOOTH_COLUMN_PROXY, &adapter, -1);

	if (adapter == NULL)
		return FALSE;

	agent = bluetooth_agent_new();

	bluetooth_agent_set_pincode_func(agent, pincode_request, adapter);
	bluetooth_agent_set_passkey_func(agent, passkey_request, adapter);
	bluetooth_agent_register(agent, adapter);

	return FALSE;
}

static void adapter_added(GtkTreeModel *model, GtkTreePath *path,
					GtkTreeIter *iter, gpointer user_data)
{
	adapter_insert(model, path, iter, user_data);
}

int setup_agents(void)
{
	dbus_g_error_domain_register(AGENT_ERROR, "org.bluez.Error",
							AGENT_ERROR_TYPE);

	client = bluetooth_client_new();

	adapter_model = bluetooth_client_get_adapter_model(client);

	g_signal_connect(G_OBJECT(adapter_model), "row-inserted",
					G_CALLBACK(adapter_added), NULL);

	gtk_tree_model_foreach(adapter_model, adapter_insert, NULL);

	return 0;
}

void cleanup_agents(void)
{
	g_object_unref(adapter_model);

	g_object_unref(client);
}

void show_agents(void)
{
}
