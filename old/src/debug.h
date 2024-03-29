#ifndef DEBUG_H
#define DEBUG_H

#ifdef __DEBUG
	#include <stdio.h>
	#include <assert.h>

	#define dprintf(x,...)	printf(x,##__VA_ARGS__)
	/*pdebug toma solamente un char * sin formato*/
	#define qstrprintf(x,...)	printf(x.toStdString().c_str(),##__VA_ARGS__)
	#define pdebug(x)	printf(x "\tin:%s\tline:%d\n",__FILE__,__LINE__)
/*nuestro assert*/
	#define ASSERT(x)	assert(x)
#else
	#define qstrprintf(x,...)	
	#define pdebug(x)	
	#define dprintf(x,...)
	#define ASSERT(x)
	
#endif

#endif

