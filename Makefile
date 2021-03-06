CC = gcc
CFLAGS = -c
LNFLAGS = -lncurses
DESTDIR ?= $(HOME)/bin
EXEC = minecurses

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(EXEC)

.PHONY: debug clean install uninstall notcurses

notcurses: LNFLAGS = -L/usr/local/lib -lnotcurses 
notcurses: CFLAGS += -I/usr/local/include -DNOTCURSES
notcurses: $(EXEC)

debug: CFLAGS += -g -gdwarf-2 -g3
debug: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LNFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXEC) *.o

install:
	install -d $(DESTDIR)
	install -m 755 $(EXEC) $(DESTDIR)

uninstall:
	rm $(DESTDIR)/$(EXEC)

