#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
	#include <assert.h>
	#define ASSERT(x)	assert(x)
#else
	#define ASSERT(x)	
#endif




#endif