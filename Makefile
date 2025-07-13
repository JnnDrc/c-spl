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


.PHONY: install uninstall test clean debug

install:
	mkdir -p $(LIBDIR)
	cp $(TARGET) $(LIBDIR)
	mkdir -p $(INCDIR)
	cp $(HEADER) $(INCDIR)

uninstall:
	rm -f $(LIBDIR)/$(TARGET)
	rm -f $(INCDIR)/$(HEADER)

# degug build without optimizations and with debug symbols
debug:
	$(CC) -c -o cspl.o cspl.c $(DBGCFLAGS)
	$(AR) libcspl-g.a cspl.o

test:
	$(CC) tests/test.c -o test -lcspl-g -L./ -g

clean:
	rm -f $(TARGET)  $(OBJ) libcspl-g.a
