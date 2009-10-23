/* Copyright (C) 2006-2007 Hendrik Sattler <post@hendrik-sattler.de>
 *       
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.		       
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *	       
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#define _GNU_SOURCE

#include "obexpushd.h"
#include "io.h"
#include "utf.h"
#include "net.h"
#include "action.h"
#include "core.h"
#include "../obex_filter/obex_filter.h"
#include "../file_manipulator/file_manipulator.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


#define WHITE_SPACES	" \n\t\r"

/*! Esta funcion se va a encargar de chequear si es vnote, de abrir el archivo
 * y modificarlo y guardar solamente el contenido que nos interesa a nosotros.
 * REQUIRES:
 *	data != NULL
*/
static void put_convert_vnote (file_data_t * fdata)
{
	uint8_t* name = NULL;
	char * vnote = NULL;
	char * clearVnote = NULL;
	VFormat * vf = NULL;
	size_t size = 0;
	
	if (!fdata)
		return;
	
	/* primero vamos a ver si es del tipo.vnote */
	name = utf16to8(fdata->name);
	if (!name)
		return;
	if (strstr(name, ".vnt") == NULL) {
		/* no es del tipo vnote */
		free (name);
		return;
	}
	{
		char buff[512];
		fdata->out = fopen (name, "r");
		
		if (!fdata->out) {
			free (name);
			return;
		}
		size = fread_unlocked((void*)buff, 1, 511, fdata->out);
		buff[size] = '\0';
		fclose (fdata->out);
		vf = vformat_new_from_string (buff);
	}
	if (!vf) {
		free (name);
		fclose (fdata->out);
		return;
	}
	vnote = vformat_to_string(vf, VFORMAT_NOTE);
	clearVnote = clean_vnote (vnote);
	
	if (!clearVnote) {
		free (name);
		vformat_free(vf);
		fclose (fdata->out);
		return;
	}
	/* ahora abrimos el archivo y guardamos el nuevo contenido */
	fdata->out = fopen (name, "w");
	if (!fdata->out) {
		fwrite (clearVnote, 1, strlen (clearVnote), fdata->out);
		fclose (fdata->out);
	}
	
	free (clearVnote);
	if (vnote)
		free(vnote);
	vformat_free(vf);
	return;
}


/*! funcion que va a leer la clave (va a leer hasta un espacio y/o un \n.
 * Probablemente sea mejor leer hasta un tamaño determinado FIXME
 * REQUIRES:
 *	fdata != NULL
 *	key[MAX_KEYS_LENGTH]
 *	nick[MAX_NICK_LENGTH]
 * RETURNS:
 *	key 		in key
 *	nickname	in nick
 *	true 		on success
 *	false 		on error
 */
static void put_get_key (file_data_t * fdata, char * key, char * nick)
{	
	uint8_t* name = NULL;
	char buff[MAX_KEYS_LENGTH + MAX_NICK_LENGTH + 2] = {0};
	
	if (!fdata || !key || !nick)
		return false;
	
	name = utf16to8(fdata->name);
	if (!name)
		return false;
	
	/* abrimos el archivo */
	fdata->out = fopen (name, "r");
		
	if (!fdata->out) {
		free (name);
		return false;
	}
	
	if (fread(buff, MAX_KEYS_LENGTH + MAX_NICK_LENGTH + 1, fdata->out) > 0){
		int i = 0;
		int j = 0;
		
		/* inicializamos la memoria */
		memset(key, '\0', MAX_KEYS_LENGTH);
		memset(nick, '\0', MAX_NICK_LENGTH);
		
		/* ahora vamos a buscar el ultimo caracter que puede ser o
		 un espacio o un \n, cualquiera de los 2 no los vamos a incluir
		 */
		while (buff[i] != '\0' && i < MAX_KEYS_LENGTH){
			if (!strchr(WHITE_SPACES, buff[i]))
				key[i] = buff[i];
			else
				break;
			i++;
		}
		while (strchr(WHITE_SPACES, buff[i]))
			i++;
		
		while (buff[i] != '\0' && j < MAX_NICK_LENGTH){		
			nick[j] = buff[i];
			j++;
			i++;
		}
	}
	free (name);
	fclose (fdata->out);
	
	return true;
	
}


static
int put_close (obex_t* handle) {
	file_data_t* data = OBEX_GetUserData(handle);

	return io_close(data);
}

static
int put_wait_for_ok (FILE* f)
{
	char ret[4+1];
	memset(ret, 0, sizeof(ret));
	fgets(ret, sizeof(ret), f);
	if (strncmp(ret, "OK\n", 3) != 0 ||
	    strncmp(ret, "OK\r\n", 4) != 0)
		return -EPERM;
	return 0;
}

static
int put_open (obex_t* handle, const char* script) {
	file_data_t* data = OBEX_GetUserData(handle);
	
	if (script != NULL && strlen(script) > 0) {
		int err = 0;
		const char* args[] = { script, "put", NULL };
		
		err = io_script_open(data, script, (char**)args);
		if (err)
			return err;
		return put_wait_for_ok(data->in);

	} else
		/*! siempre vamos a llamar a esta */
		return io_file_open(data, IO_FLAG_WRITE);
}

static
int put_write (obex_t* handle, const uint8_t* buf, int len) {
	file_data_t* data = OBEX_GetUserData(handle);
	int err;

	if (!buf)
		return -EINVAL;
	if (!data->out)
		return -EBADF;
	(void)fwrite(buf,(size_t)len,1,data->out);
	err = ferror(data->out);
	if (err)
		return -err;
	return 0;
}

static
int put_revert (obex_t* handle) {
	file_data_t* data = OBEX_GetUserData(handle);
	int err = io_close(data);

	if (!err && data->child == (pid_t)-1) {
		uint8_t* n = utf16to8(data->name);		
		if (unlink((char*)n) == -1) /* remove the file */
			err = -errno;
		if (n)
			free(n);
	}

	return err;
}

void obex_action_put (obex_t* handle, obex_object_t* obj, int event,
		       hashKeys_t * hk) {
	file_data_t* data = OBEX_GetUserData(handle);
	uint8_t* fName = NULL;
	
	
	if (data->error &&
	    (event == OBEX_EV_REQ ||
	     event == OBEX_EV_REQCHECK ||
	     event == OBEX_EV_STREAMAVAIL))
	{
		obex_send_response(handle, obj, data->error);
		return;
	}
	if (!obex_object_headers(handle,obj)) {
		obex_send_response(handle, obj, OBEX_RSP_BAD_REQUEST);
		return;
	}
	switch (event) {
	case OBEX_EV_REQHINT: /* A new request is coming in */
		(void)OBEX_ObjectReadStream(handle,obj,NULL);
		data->error = 0;
		if (data->name) {
			free(data->name);
			data->name = NULL;
		}
		if (data->type) {
			free(data->type);
			data->type = NULL;
		}
		data->count += 1;
		data->length = 0;
		data->time = 0;
		data->out = NULL;
		break;

	case OBEX_EV_REQCHECK:
		
		if (data->out == NULL
			/*! aca modificamos lo del script, no nos interesa */
		    && put_open(handle, NULL) < 0)
			obex_send_response(handle, obj, OBEX_RSP_FORBIDDEN);
		else
			obex_send_response(handle, obj, OBEX_RSP_CONTINUE);
		break;

	case OBEX_EV_STREAMAVAIL:
	{
		const uint8_t* buf = NULL;
		int len = OBEX_ObjectReadStream(handle,obj,&buf);
		
		if (data->length && data->name && (data->out == NULL))
			printf ("\n\n\nTenemos un archivo nuevo.\n\n\n");
		
		dbg_printf(data, "got %d bytes of streamed data\n", len);
		if (len) {
			if ((data->out == NULL
			     && put_open(handle, get_io_script()) < 0)
			    || put_write(handle,buf,len))
				obex_send_response(handle, obj, OBEX_RSP_FORBIDDEN);
		}
		break;
	}

	case OBEX_EV_REQDONE:
		(void)put_close(handle);
		/*!### Aca una vez que cerramos el archivo, deberiamos hacer lo
		 * siguiente:
		 * 1) Verificar si es un vnote => lo parseamos a un archivo
		 *    normal (texto plano).
		 * 2) Verificamos si es una clave => verificamos si ya esta
		 *    registrada, si esta descartamos el archivo, si no agregamos
		 *    el usuario a la DB.
		 * 3) En cualquier error lo borramos a la mierda.
		 * 4) Si no hubo error, y no era una clave, y era de un usuario
		 *    registrado => lo mandamos al sistema nuestro.
		 *###*/
		printf ("\n\nTerminamos de recibir el archivo\n\n");
		/*! convertimos si es vnote a un archivo normal, con la misma
		 * extencion. Si no es, no pasa nada
		 */
		put_convert_vnote (data);
		
		/*! verificamos si estaba registrado, si no => verificamos
		 * la clave, en caso de que sea correcta agregamos a la base
		 * de datos y mandamos el archivo al sistema */
		
		if (data->isRegistered) {
			/*! esta registrado, listo el poio mandamos el archivo
			 * de una por el conector al sistema */
			/*!### <FIXME> ### */
			printf ("El usuario esta registrado correctamente\n");
			
			
		} else {
			char key[MAX_KEYS_LENGTH];
			char nick[MAX_NICK_LENGTH];
			uint8_t* name = NULL;
			bool added = false;
			/*! no estaba registrado, se estara intentando registra?
			 * ==> verificamos si nos mando una clave y es valida
			 */
			
			if (!put_get_key (data, key, nick)) {
				printf ("El usuario mando una clave nula?\n");
			} else {
				/*! ahora verificamos si existe en la hashKeys */.
				if (hash_k_exist(hk, key)) {
					printf ("Se registro un nuevo usuario"
					" con la clave: %s\t Y nick: %s\n",
						 key, nick);
					/*! la eliminamos de la hash */
					hash_k_remove(hk, key);
					/*! registramos al usuario */
					data->isRegistered = true;
					/*! lo agregamos a la base de datos */
					added = odbc_addUser (data->MACAddr, nick);
				}
			}
			/*! si no esta registrado y sea o no sea una clave 
			 * debemos borrar lo mismo el archivo ==> */
			name = utf16to8(fdata->name);
			if (name) {
				fm_delete_file(name);
				free(name);
			}
			
		}
		
		if (data->name) {
			free(data->name);
			data->name = NULL;
		}
		if (data->type) {
			free(data->type);
			data->type = NULL;
		}
		data->length = 0;
		data->time = 0;
		/*! nos desconectamos a la mierda ...*/
		break;

	case OBEX_EV_ABORT:
		(void)put_revert(handle);
		break;
	}
}
