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

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include <dbus/dbus-glib.h>

#include <gconf/gconf-client.h>

#include <bluetooth-instance.h>
#include <bluetooth-client.h>

#include "notify.h"
#include "agent.h"

static BluetoothClient *client;
static GtkTreeModel *adapter_model;
static gboolean adapter_present = FALSE;

enum {
	ICON_POLICY_NEVER,
	ICON_POLICY_ALWAYS,
	ICON_POLICY_PRESENT,
};

static int icon_policy = ICON_POLICY_PRESENT;

#define PREF_DIR		"/apps/bluetooth-manager"
#define PREF_ICON_POLICY	PREF_DIR "/icon_policy"
#if 0
#define PREF_RECEIVE_ENABLED	PREF_DIR "/receive_enabled"
#define PREF_SHARING_ENABLED	PREF_DIR "/sharing_enabled"
#endif

static GConfClient* gconf;



static void adapter_added(GtkTreeModel *model, GtkTreePath *path,
					GtkTreeIter *iter, gpointer user_data)
{
	adapter_present = TRUE;

	if (icon_policy != ICON_POLICY_NEVER)
		show_icon();
}

static void adapter_removed(GtkTreeModel *model, GtkTreePath *path,
							gpointer user_data)
{
	if (gtk_tree_model_iter_n_children(model, NULL) < 1) {
		adapter_present = FALSE;

		if (icon_policy != ICON_POLICY_ALWAYS)
			hide_icon();
	}
}

static GConfEnumStringPair icon_policy_enum_map [] = {
	{ ICON_POLICY_NEVER,	"never"		},
	{ ICON_POLICY_ALWAYS,	"always"	},
	{ ICON_POLICY_PRESENT,	"present"	},
	{ ICON_POLICY_PRESENT,	NULL		},
};

static void gconf_callback(GConfClient *client, guint cnxn_id,
					GConfEntry *entry, gpointer user_data)
{
	GConfValue *value;

	value = gconf_entry_get_value(entry);
	if (value == NULL)
		return;

	if (g_str_equal(entry->key, PREF_ICON_POLICY) == TRUE) {
		const char *str;

		str = gconf_value_get_string(value);
		if (!str)
			return;

		gconf_string_to_enum(icon_policy_enum_map, str, &icon_policy);

		if (icon_policy == ICON_POLICY_NEVER)
			hide_icon();
		else if (icon_policy == ICON_POLICY_ALWAYS)
			show_icon();
		else if (icon_policy == ICON_POLICY_PRESENT) {
			if (adapter_present == TRUE)
				show_icon();
			else
				hide_icon();
		}
		return;
	}

#if 0
	if (g_str_equal(entry->key, PREF_RECEIVE_ENABLED) == TRUE) {
		set_receive_enabled(gconf_value_get_bool(value));
		return;
	}

	if (g_str_equal(entry->key, PREF_SHARING_ENABLED) == TRUE) {
		set_sharing_enabled(gconf_value_get_bool(value));
		return;
	}
#endif
}

static GOptionEntry options[] = {
	{ NULL },
};

int main(int argc, char *argv[])
{
	BluetoothInstance *instance;
	GtkStatusIcon *statusicon;
	GtkWidget *menu;
	GError *error = NULL;
	char *str;

	
	printf ("AAAAAAAAAAAAAAAAAAAAA\n");
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	if (gtk_init_with_args(&argc, &argv, NULL,
				options, GETTEXT_PACKAGE, &error) == FALSE) {
		if (error) {
			g_print("%s\n", error->message);
			g_error_free(error);
		} else
			g_print("An unknown error occurred\n");

		gtk_exit(1);
	}

	instance = bluetooth_instance_new("applet");
	if (instance == NULL)
		gtk_exit(0);

	g_set_application_name(_("Bluetooth Applet"));

	gtk_window_set_default_icon_name("bluetooth");

	client = bluetooth_client_new();

	adapter_model = bluetooth_client_get_adapter_model(client);

	g_signal_connect(G_OBJECT(adapter_model), "row-inserted",
					G_CALLBACK(adapter_added), NULL);

	g_signal_connect(G_OBJECT(adapter_model), "row-deleted",
					G_CALLBACK(adapter_removed), NULL);

	if (gtk_tree_model_iter_n_children(adapter_model, NULL) > 0)
		adapter_present = TRUE;

	gconf = gconf_client_get_default();

	str = gconf_client_get_string(gconf, PREF_ICON_POLICY, NULL);
	if (str) {
		gconf_string_to_enum(icon_policy_enum_map, str, &icon_policy);
		g_free(str);
	}

#if 0
	set_receive_enabled(gconf_client_get_bool(gconf,
						PREF_RECEIVE_ENABLED, NULL));

	set_sharing_enabled(gconf_client_get_bool(gconf,
						PREF_SHARING_ENABLED, NULL));
#endif

	gconf_client_add_dir(gconf, PREF_DIR, GCONF_CLIENT_PRELOAD_NONE, NULL);

	gconf_client_notify_add(gconf, PREF_DIR,
					gconf_callback, NULL, NULL, NULL);

	statusicon = init_notification();

	if (icon_policy != ICON_POLICY_ALWAYS && adapter_present == FALSE)
		hide_icon();

	
	

	setup_agents();

	gtk_main();

	gtk_widget_destroy(menu);

	g_object_unref(gconf);

	cleanup_agents();

	cleanup_notification();

	g_object_unref(adapter_model);

	g_object_unref(client);

	g_object_unref(instance);

	return 0;
}