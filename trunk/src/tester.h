#ifndef TESTER_H
#define TESTER_H

#include <assert.h>
/* funciones tipicas */
#define fail_if(x)		assert(!(x)); \
printf("fail_if (""%s"") pass \tLine:%d\n","(x)",__LINE__);
#define fail_unless(x)		assert(x); \
printf("fail_unless (" "%s" ") pass \tLine:%d\n","(x)",__LINE__);



#endif