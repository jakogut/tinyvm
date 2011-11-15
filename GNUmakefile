CC = gcc
AS = as

CFLAGS = -Wall -pipe -Iinclude/
OFLAGS = 
LFLAGS = $(CFLAGS) -Llib/
ASFLAGS =
PEDANTIC_FLAGS = -ansi -pedantic -pedantic-errors

LIBTVM_SOURCES = $(wildcard libtvm/*.c)
LIBTVM_OBJECTS = $(LIBTVM_SOURCES:.c=.o)

TDB_SOURCES = $(wildcard tdb/*.c)
TDB_OBJECTS = $(TDB_SOURCES:.c=.o)

BIN_DIR = bin
LIB_DIR = lib
INC_DIR = include
PROGRAM_DIR = programs
INSTALL_PREFIX = /usr/

DEBUG = no
PROFILE = no
PEDANTIC = no
OPTIMIZATION = -O3

ifeq ($(DEBUG), yes)
	CFLAGS += -g
	OPTIMIZATION = -O0
endif

ifeq ($(PROFILE), yes)
	CFLAGS += -pg
endif

ifeq ($(PEDANTIC), yes)
	CFLAGS += $(PEDANTIC_FLAGS)
endif

CFLAGS += $(OPTIMIZATION)

all: libtvm tvmi tdb

install: libtvm tvmi
	cp -f $(BIN_DIR)/tvmi $(INSTALL_PREFIX)/bin/
	cp -rf $(INC_DIR)/tvm $(INSTALL_PREFIX)/include/
	cp -f $(LIB_DIR)/libtvm* $(INSTALL_PREFIX)/lib/

uninstall:
	rm -rf /usr/bin/tvmi
	rm -rf /usr/include/tvmi
	rm -rf /usr/lib/libtvm*

libtvm: $(LIBTVM_OBJECTS)
	ar rcs $(LIB_DIR)/libtvm.a $(LIBTVM_OBJECTS)

# Build the TVM interpreter
tvmi: libtvm
	$(CC) $(LFLAGS) src/tvmi.c -ltvm -o $(BIN_DIR)/tvmi

tdb: libtvm $(TDB_OBJECTS)
	$(CC) $(LFLAGS) $(TDB_OBJECTS) -ltvm -o $(BIN_DIR)/tdb

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/* $(LIB_DIR)/* libtvm/*.o tdb/*.o gmon.out *.save *.o core* vgcore*

rebuild: clean all

.PHONY : clean
.SILENT : clean
