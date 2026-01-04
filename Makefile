CC := gcc
AR := ar rcs
CFLAGS := -Wall -Wextra -O3
DBGCFLAGS := -Wall -Wextra -O0 -g

PREFIX ?= /usr/local
LIBDIR := $(PREFIX)/lib
INCDIR := $(PREFIX)/include

OBJ := cspl.o
TARGET := libcspl.a
HEADER := cspl.h


$(TARGET): $(OBJ)
	$(AR) $@ $^

cspl.o: cspl.c
	$(CC) -c -o $@ $^ $(CFLAGS)


.PHONY: install uninstall examples test clean debug

install:
	mkdir -p $(LIBDIR)
	cp $(TARGET) $(LIBDIR)
	mkdir -p $(INCDIR)
	cp $(HEADER) $(INCDIR)

uninstall:
	rm -f $(LIBDIR)/$(TARGET)
	rm -f $(INCDIR)/$(HEADER)

# debug build without optimizations and with debug symbols
debug:
	$(CC) -c -o cspl.o cspl.c $(DBGCFLAGS)
	$(AR) libcspl-g.a cspl.o

examples:
	$(MAKE) debug
	$(CC) examples/basics/basics.c -o examples/basics/basics -L./ -lcspl-g -g
	$(CC) examples/writing/writing.c -o examples/writing/writing -L./ -lcspl-g -g
	rm -f examples/writing/db_new.spl

test:
	$(MAKE) debug
	$(CC) tests/test1.c -o test1 -lcspl-g -L./ -g
	$(CC) tests/test2.c -o test2 -lcspl-g -L./ -g
	$(CC) tests/test3.c -o test3 -lcspl-g -L./ -g

clean:
	rm -f $(TARGET)  $(OBJ) libcspl-g.a
