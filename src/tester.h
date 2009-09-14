#ifndef TESTER_H
#define TESTER_H

#include <assert.h>
/* funciones tipicas */
#define fail_if(x)		assert(!(x)); \
printf("fail_if ("" x " ") pass \n");
#define fail_unless(x)		assert(x)



#endif