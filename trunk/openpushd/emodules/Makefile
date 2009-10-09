CC = gcc
CFLAGS = -g -Wall -Wextra -Wdeclaration-after-statement -Wbad-function-cast \
	-Wcast-qual -Wstrict-prototypes -Wmissing-declarations \
	-Wmissing-prototypes \
	-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ \
	-D_DEBUG

LIBS = -lglib-2.0 -lm
TARGET = host
SOURCES = $(shell echo *.c)
BSTRING_OBJECTS = bstring/bstrlib.o
OBJECTS = $(SOURCES:%.c=%.o)
 
all: $(TARGET) 

# BString requiere flags distintos:
$(BSTRING_OBJECTS):CFLAGS= -ansi -Wall -Werror -pedantic -g

$(TARGET): $(OBJECTS) $(BSTRING_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

utest: $(OBJECTS) data_tests.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lcheck

-include .depend

.depend: $(SOURCES)
	$(CC) -MM $(SOURCES) > .depend
	


.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS) $(BSTRING_OBJECTS) .depend *~
