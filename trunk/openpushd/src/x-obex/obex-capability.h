#include <stdio.h>
#include <inttypes.h>
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#define bool unsigned int
#define true 1
#define false 0
#endif

struct obex_caps_version {
	char* version;
	char* date;
};

struct obex_caps_limit {
	unsigned long size_max;
	unsigned long namelen_max;
};

struct obex_caps_ext {
	char* name;
	char** value;
};

struct obex_caps_obj {
	/* type or at least one ext must be defined */
	char* type;
	char** name_ext;
	unsigned long* size;
	struct obex_caps_ext** ext; /* NULL terminated list */		
};

struct obex_caps_access {
	char* protocol;
	char* endpoint;
	char* target;
	struct obex_caps_ext** ext; /* NULL terminated list */		
};

struct obex_caps_mem {
	char* type;
	char* location;
	unsigned long* free;
	unsigned long* used;
	struct obex_caps_limit* file;
	struct obex_caps_limit* folder;
	unsigned int flags;
#define OBEX_CAPS_MEM_SHARED    (1 << 0)
#define OBEX_CAPS_MEM_CASESENSE (1 << 1)
	struct obex_caps_ext** ext; /* NULL terminated list */	
};

struct obex_caps_general {
	char* vendor;
	char* model;
	char* serial;
	char* oem;
	struct obex_caps_version* sw;
	struct obex_caps_version* fw;
	struct obex_caps_version* hw;
	char lang[2+1];
	struct obex_caps_mem** mem;
	struct obex_caps_ext** ext;
};

struct obex_caps_inbox {
	struct obex_caps_obj** obj;
	struct obex_caps_ext** ext;
};

struct obex_caps_service {
	/* name or uuid must be defined */
	char* name;
	uint8_t* uuid; /* 16 bytes */
	char* version;
	struct obex_caps_obj** obj;
	struct obex_caps_access** access;
	struct obex_caps_ext** ext;
};

struct obex_capability {
	char* charset;
	struct obex_caps_general general;
	struct obex_caps_inbox* inbox;
	struct obex_caps_service* service;
};

int obex_capability (FILE* fd, struct obex_capability* caps);
