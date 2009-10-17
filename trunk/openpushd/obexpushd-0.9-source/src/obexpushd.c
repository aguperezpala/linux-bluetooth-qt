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

#include <bluetooth/bluetooth.h>
#include "obex_auth.h"
#include "obexpushd.h"
#include "io.h"
#include "utf.h"
#include "net.h"
#include "action.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#if defined(USE_THREADS)
#include <pthread.h>
#endif

#define PROGRAM_NAME "obexpushd"
#include "version.h"
#include "compiler.h"

/* global settings */
static int debug = 0;
static int nofork = 0;
static int id = 0;
static /*@null@*/ char* auth_file = NULL;
static /*@null@*/ char* realm_file = NULL;

#define EOL(n) ((n) == '\n' || (n) == '\r')

void dbg_printf (file_data_t *data, const char *format, ...)
{
	if (debug) {
		va_list ap;
		if (data) {
			if (data->count)
				(void)fprintf(stdout, "%u.%u: ", data->id, data->count);
			else
				(void)fprintf(stdout, "%u: ", data->id);
		}
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);
	}
}

/* return len(> 0), 0 if not found, or err codes(< 0) */
static
ssize_t get_pass_for_user (char* file,
			   const uint8_t* user, size_t ulen,
			   /*@out@*/ uint8_t* pass, size_t size)
{
	ssize_t ret = 0;
	size_t lsize = ulen+1+size+3;
	char* line = malloc(lsize);
	int status = 0;
	FILE* f;
	
	if (!line)
		return -ENOMEM;
	if (file[0] == '|') {
		const char* args[2] = { file+1, (const char*)user };
		int fds[2];
		int err = pipe_open(args[0], (char**)args, fds, NULL);
		if (err)
			return err;
		close(fds[1]);
		status = fds[0];
	} else {
		status = open(file, O_RDONLY);
		if (status == -1)
			status = -errno;
	}
	if (status < 0) {
		free(line);
		return status;
	}
	f = fdopen(status,"r");
	if (!f) {
		ret = (ssize_t)-errno;
		free(line);
		return ret;
	}
	while (1) {
		size_t len = 0;
		if (fgets(line,(int)lsize,f) == NULL)
			break;
		len = strlen(line);

		/* test that we read a whole line */
		if (!EOL(line[len-1])) {
			ret = -EINVAL; /* password in file too large */
			break;
		}

		if (line[len-1] == '\n')
			--len;
		if (line[len-1] == '\r')
			--len;

		if (ulen > len ||
		    memcmp(line,user,ulen) != 0 ||
		    line[ulen] != ':')
			continue;
		/* since the above matches the user id and the delimiter
		 * the rest of the line must be the password
		 */
		ret = (ssize_t)(len-ulen-1);
		if ((size_t)ret > size) {
			ret = -EINVAL; /* password in file too large */
			break;
		}
		memcpy(pass,line+ulen+1,(size_t)ret);
	}

	(void)fclose(f);
	free(line);
	return ret;
}

static
int obex_auth_verify_response (obex_t __unused *handle,
			       obex_headerdata_t h,
			       uint32_t size)
{
	struct obex_auth_response resp;
	uint8_t pass[1024];
	int len;

	if (!auth_file)
		return 0;

	memset(&resp,0,sizeof(resp));
	memset(pass,0,sizeof(pass));

	if (obex_auth_unpack_response(h,size,&resp) < 0)
		return 0;
	len = (int)get_pass_for_user(auth_file,resp.user,resp.ulen,pass,sizeof(pass));
	if (len < 0)
		return 0;
	return obex_auth_check_response(&resp,pass,(size_t)len);
}

/* return len(> 0), 0 if not found, or err codes(< 0) */
static
ssize_t get_credentials_for_realm (char* file,
				   const uint8_t* realm,
				   /*@out@*/ uint8_t* user, size_t* usize,
				   /*@out@*/ uint8_t* pass, size_t* psize)
{
	ssize_t ret = 0;
	size_t size = *usize+1+*psize+1;
	uint8_t* buffer = malloc(size);
	uint8_t* r = NULL;
	
	if (!buffer)
		return -ENOMEM;
	/* the format for both files is basicly the same */
	ret = get_pass_for_user(file,realm,utf8len(r),buffer,size);

	if (ret > 0) {
		r = (uint8_t*)strchr((char*)buffer,(int)':');
		if (r == NULL ||
		    (usize != 0 && (size_t)(r-buffer) > *usize) ||
		    (size_t)((buffer+ret)-(r+1)) > *psize) {
			free(buffer);
			return -EINVAL;
		}
		if (usize) {
			*usize = (size_t)(r-buffer);
			if (user)
				memcpy(user,buffer,*usize);
		}

		*psize = (size_t)((buffer+ret)-(r+1));
		memcpy(pass,r+1,*psize);
	}
	free(buffer);
	return ret;
}

static
void get_creds (obex_t __unused *handle,
		const uint16_t* realm, /* UTF-16 */
		/*out*/ char* user,
		size_t* ulen,
		/*out*/ char* pass,
		size_t* plen)
{
	uint8_t* realm8 = utf16to8(realm);
	get_credentials_for_realm(realm_file,
				  (const uint8_t*)realm8,
				  (uint8_t*)user,ulen,
				  (uint8_t*)pass,plen);
	free(realm8);
}

static
int obex_auth_send_response (obex_t* handle,
			     obex_object_t* obj,
			     obex_headerdata_t h,
			     uint32_t size)
{
	struct obex_auth_challenge chal;
	struct obex_auth_response resp;
	ssize_t len;
	
	if (!realm_file)
		return -EINVAL;
	memset(&chal,0,sizeof(chal));
	len = (ssize_t)obex_auth_unpack_challenge(h,size,&chal,1);
	if (len < 0)
		return -EINVAL;
	obex_auth_challenge2response(handle,&chal,&resp,get_creds);
	free(chal.realm);
	return obex_auth_add_response(handle,obj,&resp);
}

static const char* obex_event_string(uint8_t event)
{
	static const char* obex_events[] = {
		"PROGRESS", "REQHINT", "REQ", "REQDONE",
		"LINKERR", "PARSEERR", "ACCEPTHINT", "ABORT",
		"STREAMEMPTY", "STREAMAVAIL", "UNEXPECTED", "REQCHECK",
	};

	return obex_events[event];
}

static const char* obex_command_string(uint8_t cmd)
{
	static const char* obex_commands[] = {
		"CONNECT", "DISCONNECT", "PUT", "GET",
		"SETPATH", "SESSION", "ABORT"
	};

	switch (cmd) {
	case OBEX_CMD_CONNECT:
	case OBEX_CMD_DISCONNECT:
	case OBEX_CMD_PUT:
	case OBEX_CMD_GET:
		return obex_commands[cmd];
	case OBEX_CMD_SETPATH:
		return obex_commands[4];
	case OBEX_CMD_SESSION:
		return obex_commands[5];
	case OBEX_CMD_ABORT:
		return obex_commands[6];
	default:
		return "UNKNOWN";
	}
}

int obex_object_headers (obex_t* handle, obex_object_t* obj) {
	uint8_t id = 0;
	obex_headerdata_t value;
	uint32_t vsize;
	file_data_t* data = OBEX_GetUserData(handle);

	while (OBEX_ObjectGetNextHeader(handle,obj,&id,&value,&vsize)) {
		dbg_printf(data, "Got header 0x%02x with value length %u\n",
			   (unsigned int)id, (unsigned int)vsize);
		if (!vsize)
			continue;
		switch (id) {
		case OBEX_HDR_NAME:
			if (data) {
				if (data->name)
					free(data->name);
				data->name = malloc(vsize+2);
				if (!data->name)
					return 0;
				memset(data->name,0,vsize+2);
				memcpy(data->name,value.bs,vsize);
				ucs2_ntoh(data->name,vsize/2);
				if (debug) {
					uint8_t* n = utf16to8(data->name);
					dbg_printf(data, "name: \"%s\"\n", (char*)n);
					free(n);
				}
				if (!check_name(data->name)) {
					dbg_printf(data, "CHECK FAILED: %s\n", "Invalid name string");
					return 0;
				}
			}
			break;

		case OBEX_HDR_TYPE:
			if (data) {
				if (data->type)
					free(data->type);
				data->type = malloc(vsize+1);
				if (!data->type)
					return 0;
				memcpy(data->type,value.bs,vsize);
				data->type[vsize] = '\0';
				dbg_printf(data, "type: \"%s\"\n", data->type);
				if (!check_type(data->type)) {
					dbg_printf(data, "CHECK FAILED: %s\n", "Invalid type string");
					return 0;
				}
			}
			break;

		case OBEX_HDR_LENGTH:
			if (data)
				data->length = value.bq4;
			dbg_printf(data, "size: %d bytes\n", value.bq4);
			break;

		case OBEX_HDR_TIME:
			/* ISO8601 formatted ASCII string */
			if (data) {
				struct tm time;
				char* tmp = malloc(vsize+1);
				if (!tmp)
					return 0;
				memcpy(tmp, value.bs, vsize);
				tmp[vsize] = '\0';
				dbg_printf(data, "time: \"%s\"\n", tmp);
				tzset();
				strptime(tmp, "%Y-%m-%dT%H:%M:%S", &time); /* uses GNU extensions */
				time.tm_isdst = -1;
				data->time = mktime(&time);
				if (tmp[17] == 'Z')
					data->time -= timezone;
				free(tmp);
				tmp = NULL;
			}
			break;

		case OBEX_HDR_DESCRIPTION:
			if (debug) {
				uint16_t* desc16 = (uint16_t*)value.bs;
				if (desc16[vsize/2] == 0x0000) {
					uint8_t* desc8 = utf16to8(desc16);
					dbg_printf(data, "description: \"%s\"\n", (char*)desc8);
					free(desc8);
				}
			}
			break;

		case OBEX_HDR_AUTHCHAL:
			if (realm_file && data->net_data->auth_success)
				(void)obex_auth_send_response(handle,obj,value,vsize);
			break;

		case OBEX_HDR_AUTHRESP:
			data->net_data->auth_success = obex_auth_verify_response(handle,value,vsize);
			break;

		default:
			/* some unexpected header, may be a bug */
			break;
		}
	}
	return 1;
}

void obex_send_response (obex_t* handle, obex_object_t* obj, uint8_t respCode) {
	switch (respCode) {
	case OBEX_RSP_CONTINUE:
	case OBEX_RSP_SUCCESS:
		(void)OBEX_ObjectSetRsp(obj,
					OBEX_RSP_CONTINUE,
					OBEX_RSP_SUCCESS);
		break;

	default:
	        {
			file_data_t* data = OBEX_GetUserData(handle);
			(void)OBEX_ObjectSetRsp(obj, respCode, respCode);
			data->error = respCode;
			break;
		}
	}
}

static
void client_eventcb (obex_t* handle, obex_object_t* obj,
		     int __unused mode, int event,
		     int obex_cmd, int __unused obex_rsp)
{
	file_data_t* data = OBEX_GetUserData(handle);
	static int last_obex_cmd = 0;

	/* work-around for openobex bug */
	if (event == OBEX_EV_STREAMAVAIL ||
	    event == OBEX_EV_STREAMEMPTY)
		obex_cmd = last_obex_cmd;
	else
		last_obex_cmd = obex_cmd;

	if (debug)
		printf("%u: OBEX_EV_%s, OBEX_CMD_%s\n", data->id,
		       obex_event_string(event), obex_command_string(obex_cmd));

	switch (obex_cmd) {
	case OBEX_CMD_CONNECT:
		obex_action_connect(handle,obj,event);
		break;

	case OBEX_CMD_PUT:
		if (net_security_check(data->net_data))
			obex_action_put(handle,obj,event);
		break;

	case OBEX_CMD_GET:
		if (net_security_check(data->net_data))
			obex_action_get(handle,obj,event);
		break;

	case OBEX_CMD_DISCONNECT:
		obex_action_disconnect(handle,obj,event);
		break;

	case OBEX_CMD_ABORT:
		if (last_obex_cmd == OBEX_CMD_PUT) {
			obex_action_put(handle,NULL,OBEX_EV_ABORT);
		}
		break;

	default:
		switch (event) {
		case OBEX_EV_REQHINT: /* A new request is coming in */
			/* Reject any other commands */                       
			obex_send_response(handle, obj, OBEX_RSP_NOT_IMPLEMENTED);
			break;
		}
	}
}

static void* handle_client (void* arg) {
	file_data_t* data = malloc(sizeof(*data));
	char buffer[256];

	if (!data)
		goto out1;
	memset(data,0,sizeof(*data));
	data->id = id++;
	data->child = (pid_t)-1;

	data->net_data = malloc(sizeof(*data->net_data));
	if (!data->net_data)
		goto out2;
	memcpy(data->net_data, OBEX_GetUserData(arg), sizeof(*data->net_data));
	data->net_data->obex = arg;
	if (!data->net_data->obex)
		goto out2;
	OBEX_SetUserData(data->net_data->obex, data);

	memset(buffer, 0, sizeof(buffer));
	net_get_peer(data->net_data, buffer, sizeof(buffer));
	fprintf(stderr,"Connection from \"%s\"\n", buffer);

	do {
		if (OBEX_HandleInput(data->net_data->obex, 10) < 0)
			break;
	} while (1);

out2:
	if (data) {
		if (data->net_data) {
			OBEX_Cleanup(data->net_data->obex);
			free(data->net_data);
		}
		if (data->name)
			free(data->name);
		if (data->type)
			free(data->type);
		free(data);
	}
out1:
	return NULL;
}

static
void eventcb (obex_t* handle, obex_object_t __unused *obj,
	      int __unused mode, int event,
	      int __unused obex_cmd, int __unused obex_rsp)
{
	dbg_printf(NULL, "OBEX_EV_%s, OBEX_CMD_%s\n",
		   obex_event_string(event), obex_command_string(obex_cmd));
	if (obj && !obex_object_headers(handle,obj)) {
		(void)OBEX_ObjectSetRsp(obj,
					OBEX_RSP_BAD_REQUEST,
					OBEX_RSP_BAD_REQUEST);
		return;
	}
	if (event == OBEX_EV_ACCEPTHINT) {
		int fd;
		obex_t* client = OBEX_ServerAccept(handle, client_eventcb, NULL);
		if (!client)
			return;
		fd = OBEX_GetFD(client);
		if (fd >= 0)
			(void)fcntl(fd, F_SETFD, FD_CLOEXEC);
		if (nofork >= 2) {
			(void)handle_client(client);
		} else {
#if defined(USE_THREADS)
			pthread_t t;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			if (pthread_create(&t, &attr, handle_client, client) != 0)
				perror("pthread_create()");

#else
			pid_t p = fork();
			switch (p) {
			case 0:
				(void)signal(SIGCHLD, SIG_DFL);
				(void)signal(SIGINT, SIG_DFL);
				(void)signal(SIGTERM, SIG_DFL);
				(void)handle_client(client);
				exit(EXIT_SUCCESS);

			case -1:
				perror("fork()");
			}
#endif
		}
	}
}

#if defined(USE_THREADS)
static void obexpushd_listen_thread_cleanup (void* arg) {
	net_cleanup(arg);
}

static void* obexpushd_listen_thread (void* arg) {
	struct net_data* data = arg;

	pthread_cleanup_push(obexpushd_listen_thread_cleanup, arg);
	net_init(data, eventcb);
	if (!data->obex) {
		fprintf(stderr, "net_init() failed\n");
		pthread_exit(NULL);
	}
	do {
		if (OBEX_HandleInput(data->obex, 3600) < 0) {
			/* OpenOBEX sometimes return -1 anyway, must be a bug
			 * thus the break is commented -> go on anyway
			 */
			//break;
		}
	} while (1);
	pthread_cleanup_pop(1);
	return NULL;
}
#endif

static void print_disclaimer () {
	printf(PROGRAM_NAME " " OBEXPUSHD_VERSION " Copyright (C) 2006-2009 Hendrik Sattler\n"
	       "This software comes with ABSOLUTELY NO WARRANTY.\n"
	       "This is free software, and you are welcome to redistribute it\n"
	       "under certain conditions.\n");
}

static void print_help (char* me) {
	print_disclaimer();
	printf("\n");
	printf("Usage: %s [<interfaces>] [<options>]\n", me);
	printf("\n"
	       "Interfaces:\n"
	       " -B[<channel>]  listen to bluetooth connections (default: channel 9)\n"
	       " -I[<app>]      listen to IrDA connections (app example: IrXfer)\n"
#if OPENOBEX_TCPOBEX
	       " -N[<port>]     listen to IPv4/v6 network connections (default: port 650)\n"
#else
	       " -N             listen to IPv4 network connections on port 650\n"
#endif
	       "\n"
	       "Options:\n"
	       " -n             do not detach from terminal\n"
	       " -d             enable debug messages (implies -n)\n"
	       " -p <file>      write pid to file when getting detached\n"
	       " -A             use transport layer specific access rules if available\n"
	       " -a <file>      authenticate against credentials from file (EXPERIMENTAL)\n"
	       " -r <file>      use realm credentials from file (EXPERIMENTAL)\n"
	       " -s <file>      define script/program for input/output\n"
	       " -h             this help message\n"
	       " -v             show version\n");
	printf("\n"
	       "See manual page %s(1) for details.\n",me);
}

static char* parse_bluetooth_arg (char* optarg, uint8_t* btchan)
{
	char* tmp;
	char* device = NULL;

	if (strlen(optarg) >= 19 && optarg[0] == '[' && optarg[18] == ']') {
		device = optarg+1;
		device[17] = 0;
		tmp = optarg+19;

	} else if (strncmp(optarg, "hci", 3) == 0) {
		device = optarg;
		tmp = strchr(optarg, (int)':');

	} else {
		tmp = optarg;
	}

	if (tmp) {
		long arg = 0;
		if (*tmp == ':') {
			*tmp = 0;
			++tmp;
		}
		arg = strtol(tmp, NULL, 10);
		if (0 < arg && arg < (1 << 8))
			*btchan = (uint8_t)arg;
	}

	return device;
}

#if OPENOBEX_TCPOBEX
static char* parse_ip_arg (char* optarg, uint16_t* port)
{
	char* device = NULL;

	char* tmp = strchr(optarg, (int)']');
	if (tmp && optarg[0] == '[') { /* IPv6 address */
		device = optarg+1;
		*tmp = 0;
		++tmp;

	} else if (strchr(optarg, (int)'.') != NULL) { /* IPv4 address */
		device = optarg;
		tmp = strchr(optarg, (int)':');

	} else if (optarg[0] == '*' && optarg[1] == ':') {
		device = optarg;
		tmp = optarg+1;
		
	} else { /* no address */
		tmp = optarg;
	}

	if (tmp) {
		long portnum = 0;
		if (*tmp == ':') {
			*tmp = 0;
			++tmp;
		}
		portnum = strtol(tmp, NULL, 10);
		if (portnum > 0 && portnum < (1 << 16))
			*port = portnum;
	}
	return device;
}
#endif

static struct net_data* handle[4] = {
	NULL, NULL, NULL, NULL
};
#define BT_HANDLE         handle[0]
#define IRDA_HANDLE       handle[1]
#define IRDA_EXTRA_HANDLE handle[2]  
#define INET_HANDLE       handle[3]

static void obexpushd_shutdown (int sig) {
	size_t i;
	(void)signal(SIGINT, SIG_DFL);
	(void)signal(SIGTERM, SIG_DFL);
	for (i = 0; i < sizeof(handle)/sizeof(*handle); ++i) {
		if (handle[i]) {
			struct net_data* h = handle[i];
			handle[i] = NULL;
			net_cleanup(h);
		}
	}
	(void)kill(getpid(), sig);
}

#if ! defined(USE_THREADS)
static void obexpushd_wait (int sig) {
	pid_t pidOfChild;
	int status;
	if (sig != SIGCLD)
		return;

	pidOfChild = wait(&status);
	if (WIFEXITED(status))
		fprintf(stderr, "child exited with exit code %d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		fprintf(stderr, "child got signal %d\n", WTERMSIG(status));
}
#endif

int main (int argc, char** argv) {
	size_t i;
	char* pidfile = NULL;
#if defined(USE_THREADS)
	pthread_t thread[sizeof(handle)/sizeof(*handle)];
#endif
	uint8_t auth_level = 0;

	int c;
	while ((c = getopt(argc,argv,"B::I::N::Aa:dhnp:r:s:v")) != -1) {
		switch (c) {
		case 'B':
		{
			char* device = NULL;
			uint8_t btchan = 9;
			if (BT_HANDLE)
				net_cleanup(BT_HANDLE);
			BT_HANDLE = net_data_new();
			if (optarg) {
				device = parse_bluetooth_arg(optarg, &btchan);
			}
			if (bluetooth_setup(BT_HANDLE, device, btchan)) {
				net_cleanup(BT_HANDLE);
				BT_HANDLE = NULL;
			}
			break;
		}

		case 'I':
			if (IRDA_EXTRA_HANDLE) {
				net_cleanup(IRDA_EXTRA_HANDLE);
				IRDA_EXTRA_HANDLE = NULL;
			}
			if (IRDA_HANDLE)
				net_cleanup(IRDA_HANDLE);
			IRDA_HANDLE = net_data_new();
			if (irda_setup(IRDA_HANDLE, NULL)) {
				net_cleanup(IRDA_HANDLE);
				IRDA_HANDLE = NULL;
			}
			if (optarg) {
				IRDA_EXTRA_HANDLE = net_data_new();
				if (irda_setup(IRDA_EXTRA_HANDLE, optarg)) {
					net_cleanup(IRDA_EXTRA_HANDLE);
					IRDA_EXTRA_HANDLE = NULL;
				}
			}
			break;

		case 'N':
		{
#if OPENOBEX_TCPOBEX
			char* address = NULL;
			uint16_t port = 650;

			if (optarg) {
				address = parse_ip_arg(optarg, &port);
			}
#endif
			if (INET_HANDLE)
				net_cleanup(INET_HANDLE);
			INET_HANDLE = net_data_new();
#if OPENOBEX_TCPOBEX
			if (tcp_setup(INET_HANDLE, address, port))
#else
			if (inet_setup(INET_HANDLE))
#endif
			{
				net_cleanup(INET_HANDLE);
				INET_HANDLE = NULL;
			}
			break;
		}

		case 'd':
			debug = 1;
			/* no break */

		case 'n':
			++nofork;
			break;

		case 'p':
			pidfile = optarg;
			break;

		case 'A':
			auth_level |= AUTH_LEVEL_TRANSPORT;
			break;

		case 'a':
			auth_file = optarg;
			auth_level |= AUTH_LEVEL_OBEX;
			break;

		case 'r':
			realm_file = optarg;
			break;

		case 's':
			set_io_script(optarg);
			break;

		case 'h':
			print_help(PROGRAM_NAME);
			exit(EXIT_SUCCESS);

		case 'v':
			printf("%s\n",OBEXPUSHD_VERSION);
			exit(EXIT_SUCCESS);
		}
	}

	/* check that at least one listener was enabled */
	for (i = 0; i < sizeof(handle)/sizeof(*handle); ++i) {
		if (handle[i])
			break;
	}
	if (i == sizeof(handle)/sizeof(*handle)) {
		BT_HANDLE = net_data_new();
		if (bluetooth_setup(BT_HANDLE, NULL, 9)) {
			net_cleanup(BT_HANDLE);
			exit(EXIT_FAILURE);
		}
	}
		
	/* fork if allowed (detach from terminal) */
	if (nofork < 1) {
		if (daemon(1,0) < 0) {
			perror("daemon()");
			exit(EXIT_FAILURE);
		}
		if (pidfile) {
			FILE* p = fopen(pidfile,"w+");
			if (p) {
				fprintf(p,"%u\n",(unsigned int)getpid());
				(void)fclose(p);
			}
		}
	} else {
		print_disclaimer();
	}

	/* setup the signal handlers */
	(void)signal(SIGINT, obexpushd_shutdown);
	(void)signal(SIGTERM, obexpushd_shutdown);

#if defined(USE_THREADS)
	/* initialize all enabled listeners */
	for (i = 0; i < sizeof(handle)/sizeof(*handle); ++i) {
		if (!handle[i])
			continue;
		handle[i]->auth_level = auth_level;
		if (pthread_create(&thread[i], NULL, obexpushd_listen_thread, handle[i]) != 0)
			perror("pthread_create()");
	}

	for (i = 0; i < sizeof(handle)/sizeof(*handle); ++i) {
		if (handle[i]) {
			void* retval;
			pthread_join(thread[i], &retval);
		}
	}
	pthread_exit(NULL);

#else
	(void)signal(SIGCHLD, obexpushd_wait);

	/* initialize all enabled listeners */
	for (i = 0; i < sizeof(handle)/sizeof(*handle); ++i) {
		int fd = -1;
		if (!handle[i])
			continue;
		handle[i]->auth_level = auth_level;
		net_init(handle[i], eventcb);
		if (!handle[i]->obex)
			exit(EXIT_FAILURE);
		fd = OBEX_GetFD(handle[i]->obex);
		if (fd == -1) {
			perror("OBEX_GetFD()");
			exit(EXIT_FAILURE);
		}
	}
	
	/* run the multiplexer */
	do {
		int topfd = 0;
		fd_set fds;
		FD_ZERO(&fds);
		for (i = 0; i < sizeof(handle)/sizeof(*handle); ++i) {
			if (!handle[i])
				continue;
			if (handle[i]->obex) {
				int fd = OBEX_GetFD(handle[i]->obex);
				if (fd == -1) {
					perror("OBEX_GetFD()");
					exit(EXIT_FAILURE);
				}
				if (fd > topfd)
					topfd = fd;
				FD_SET(fd, &fds);
			}
		}
		select(topfd+1, &fds, NULL, NULL, NULL);
		for (i = 0; i < sizeof(handle)/sizeof(*handle); ++i) {
			int fd = -1;
			if (!handle[i])
				continue;
			if (!handle[i]->obex)
				continue;
			fd = OBEX_GetFD(handle[i]->obex);
			if (FD_ISSET(fd,&fds))
				(void)OBEX_HandleInput(handle[i]->obex,1);
		}			
	} while (1);
#endif

	/* never reached */
	return EXIT_FAILURE;
}