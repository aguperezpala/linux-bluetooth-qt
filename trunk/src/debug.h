#ifndef DEBUG_H
#define DEBUG_H

#ifdef __DEBUG
	#include <stdio.h>
	#define dprintf(x,...) printf(x,##__VA_ARGS__)
#else
	#define dprintf(x,...) 
#endif

#endif
