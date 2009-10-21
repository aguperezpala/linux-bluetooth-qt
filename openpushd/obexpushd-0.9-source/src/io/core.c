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

#define _POSIX_SOURCE

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <utime.h>

#include "obexpushd.h"
#include "io.h"
#include "utf.h"
#include "net.h"

int io_close (file_data_t* data)
{
	if (data->child != (pid_t)-1) {
		int status;

		kill(data->child, SIGKILL);
		if (waitpid(data->child, &status, 0) < 0)
			return -errno;

		data->child = (pid_t)-1;

		if (WIFEXITED(status)) {
			fprintf(stderr, "%u.%u: script exited with exit code %d\n",
				data->id, data->count, WEXITSTATUS(status));

		} else if (WIFSIGNALED(status)) {
			fprintf(stderr, "%u.%u: script got signal %d\n",
				data->id, data->count, WTERMSIG(status));
		}

		//prevent call of utime below
		data->time = 0;
	}

	if (data->in) {
		if (fclose(data->in) == EOF)
			return -errno;
		data->in = NULL;
	}

	if (data->out) {
		if (fclose(data->out) == EOF)
			return -errno;
		if (data->time) {
			char* name = (char*)utf16to8(data->name);
			struct utimbuf times;

			times.actime = data->time;
			times.modtime = data->time;
			if (utime(name, &times) == -1)
				perror("Setting time failed");
			free(name);
		}			
		data->out = NULL;
	}

	return 0;
}

int io_script_open (file_data_t* data, const char* script, char** args)
{
	int err = 0;
	int p[2] = { -1, -1};
	char from[256];
	uint8_t* name = utf16to8(data->name);

	if (!name)
		return -EINVAL;

	err = io_close(data);
	if (err)
		return err;

	err = pipe_open(script, args, p, &data->child);
	if (err)
		return err;

	data->in = fdopen(p[0], "r");
	if (data->in)
		data->out = fdopen(p[1], "w");
	if (!data->in || !data->out) {
		err = errno;
		pipe_close(p);
		io_close(data);
		return -err;
	}

	memset(from, 0, sizeof(from));
	net_get_peer(data->net_data, from, sizeof(from));

	/* headers can be written here */
	fprintf(data->out, "From: %s\n", (strlen(from)? from: "unknown"));
	fprintf(data->out, "Name: %s\n", name);
	if (data->length)
		fprintf(data->out, "Length: %zu\n", data->length);
	if (data->type)
		fprintf(data->out, "Type: %s\n", data->type);

	free(name);
	
	/* empty line signals that data follows */
	fprintf(data->out, "\n");
	fflush(data->out);

	return err;
}

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

int io_file_open (file_data_t* data, unsigned long io_flags)
{
	int err;
	uint8_t* name = utf16to8(data->name);

	if (!name)
		return -EINVAL;

	err = io_close(data);
	if (err)
		return err;

	if (io_flags & IO_FLAG_WRITE) {
		printf("%u.%u: Creating file \"%s\"\n", data->id, data->count, (char*)name);
		err = open((char*)name, O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,
			      S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
		if (err == -1) {
			fprintf(stderr, "%u.%u: Error: cannot create file: %s\n",
				data->id, data->count, strerror(-err));
			goto io_file_error;
		}
#if ! O_CLOEXEC
		(void)fcntl(err, F_SETFD, FD_CLOEXEC);
#endif
		data->out = fdopen(err, "w");
		if (data->out == NULL)
			goto io_file_error;
	}

	if (io_flags & IO_FLAG_READ) {
		err = open((char*)name, O_RDONLY|O_CLOEXEC);
		if (err == -1)
			goto io_file_error;
		data->in = fdopen(err, "r");
		if (data->in == NULL)
			goto io_file_error;
	}

	free(name);
	return 0;

io_file_error:
	err = -errno;
	free(name);
	(void)io_close(data);

	return err;
}