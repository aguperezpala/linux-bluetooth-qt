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

static GOptionEntry options[] = {
	{ NULL },
};

int main(int argc, char *argv[])
{
	BluetoothInstance *instance;
	GError * error;


	
	printf ("AAAAAAAAAAAAAAAAAAAAA\n");
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	if (gtk_init_with_args(&argc, &argv, NULL,
		options, GETTEXT_PACKAGE, &error) == FALSE) {
		if (error) {
			g_print("******%s\n", error->message);
			g_error_free(error);
		} else
			g_print("An unknown error occurred\n");
		
		gtk_exit(1);
	}
	instance = bluetooth_instance_new("applet");
	if (instance == NULL) {
		printf ("Main: Error creando instancia\n");
		return 1;
	}
	
	g_set_application_name(_("Bluetooth Applet"));

	setup_agents();

	gtk_main();
	cleanup_agents();	
	cleanup_notification();
	g_object_unref(instance);
	
	return 0;
}
