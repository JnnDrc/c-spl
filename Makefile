CC := gcc
AR := ar rcs
CFLAGS := -Wall -Wextra -O3

TARGET := libcspl.a
OBJ := cspl.o

$(TARGET): $(OBJ)
	$(AR) $@ $^

cspl.o: cspl.c
	$(CC) -c -o $@ $^ $(CFLAGS)


.PHONY: test clean debug

debug:
	$(CC) -c -g -o cspl.o cspl.c $(CFLAGS)
	$(AR) libcspl-g.a cspl.o

test:
	$(CC) tests/test.c -o test -lcspl-g -L./ -g

clean:
	rm -f $(TARGET)  $(OBJ) libcspl-g.a
