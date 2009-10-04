TARGET=ksamp
CC=gcc
CFLAGS+= -lbluetooth -ansi -Wall -Wextra -Wdeclaration-after-statement \
-Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-declarations \
-Wmissing-prototypes -Wno-unused-parameter -Werror -pedantic -g
SOURCES=$(shell echo *.c)
OBJECTS=$(SOURCES:.c=.o)

all: $(TARGET)

# BString requiere flags distintos:


$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~


.depend: *.[ch]
	$(CC) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all
