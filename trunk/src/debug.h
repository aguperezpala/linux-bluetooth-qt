#ifndef DEBUG_H
#define DEBUG_H

/* definimos algunas funciones */
#ifdef __DEBUG
#include <assert.h>

#define debugp(x)	printf(x)
#define ASSERT(x)	assert(x)
#else
#define debugp(x)	
#define ASSERT(x)	
#endif

/* funcion que saca el char * de un QString */
#define qstrtochar(x)	x.toStdString().c_str()


#endif
