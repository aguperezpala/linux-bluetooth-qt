CC = gcc
CFLAGS = 
LIBS = -lbluetooth -lopenobex
TARGET = server
SOURCES = $(shell echo *.c)
OBJECTS = $(SOURCES:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

-include .depend

.depend: $(SOURCES)
	$(CC) -MM $(SOURCES) > .depend

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~
