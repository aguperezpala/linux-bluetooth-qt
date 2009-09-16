#ifndef DATA_IO_H
#define DATA_IO_H
#include <sys/types.h>

int file_open (
	char* name,
	int mode
);

pid_t pipe_open (
	const char* command,
	char** args, 
	int client_fds[2]
);
void pipe_close (int client_fds[2]);
#endif
