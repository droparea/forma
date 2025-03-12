
ifeq ($(OS),Windows_NT)
PATH_MINGW := /c//MinGW64
CC     := PATH="$(PATH_MINGW)/bin;$(PATH)" $(PATH_MINGW)/bin/gcc -m32 -static
GDB    := PATH="$(PATH_MINGW)/bin;$(PATH)" $(PATH_MINGW)/bin/gdb
else
CC     := gcc
GDB    := gdb
endif

CFLAGS :=
CFLAGS += -g
CFLAGS += -pedantic
CFLAGS += -Wall
CFLAGS += -Wunused-parameter
CFLAGS += -Wunused-variable
CFLAGS += -Wreturn-type
CFLAGS += -Wextra 
CFLAGS += -Wunreachable-code
CFLAGS += -Wunused
CFLAGS += -Wundef
CFLAGS += -Wfloat-equal
CFLAGS += -Wcast-align
CFLAGS += -Wredundant-decls
CFLAGS += -Winline
CFLAGS += -Wshadow
CFLAGS += -Wwrite-strings
CFLAGS += -Wmissing-format-attribute
CFLAGS += -Werror


all:
	mkdir -p \!outputs
	rm -f \!outputs/*.c.o
	$(CC) $(CFLAGS) -c -o \!outputs/forma.c.o                ../src/forma.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv.c.o           ../src/forma_conv.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_base64.c.o    ../src/forma_conv_base64.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_char.c.o      ../src/forma_conv_char.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_float.c.o     ../src/forma_conv_float.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_function.c.o  ../src/forma_conv_function.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_hex.c.o       ../src/forma_conv_hex.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_int.c.o       ../src/forma_conv_int.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_net.c.o       ../src/forma_conv_net.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_str.c.o       ../src/forma_conv_str.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_conv_time.c.o      ../src/forma_conv_time.c
	$(CC) $(CFLAGS) -c -o \!outputs/forma_writer.c.o         ../src/forma_writer.c
	$(CC) -I../../../core -o \!outputs/forma_test \!outputs/*.c.o forma_test.c
	./\!outputs/forma_test


gdb:
	$(GDB) ./\!outputs/forma_test


