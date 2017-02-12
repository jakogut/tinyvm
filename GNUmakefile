CC = clang

CFLAGS = -Wall -pipe -Iinclude/ -std=gnu11
OFLAGS = 
LFLAGS = $(CFLAGS) -Llib/
PEDANTIC_FLAGS = -Werror -pedantic -pedantic-errors
FREESTANDING_FLAGS = -nostdlib -nodefaultlibs -fno-builtin -Imlibc/include

LIBTVM_SOURCES = $(wildcard libtvm/*.c)
LIBTVM_OBJECTS = $(LIBTVM_SOURCES:.c=.o)

TDB_SOURCES = $(wildcard tdb/*.c)
TDB_OBJECTS = $(TDB_SOURCES:.c=.o)

BIN_DIR = bin
LIB_DIR = lib
INC_DIR = include
PROGRAM_DIR = programs
INSTALL_PREFIX = /usr

DEBUG = no
PEDANTIC = yes
OPTIMIZATION = -O3

FREESTANDING = no

PROFILER = operf
PROF_ARGS = "programs/tinyvm/euler/euler7.vm"

ifeq ($(DEBUG), yes)
	CFLAGS += -g
endif

ifeq ($(PEDANTIC), yes)
	CFLAGS += $(PEDANTIC_FLAGS)
endif

ifeq ($(FREESTANDING), yes)
	LFLAGS += -Lmlibc/lib -lminimalc
	CFLAGS += $(FREESTANDING_FLAGS)
	FREESTANDING_DEPS = mlibc
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

submodules:
	git submodule update --init

mlibc: submodules
	(cd mlibc && $(MAKE))

# Build the TVM interpreter
tvmi: libtvm $(FREESTANDING_DEPS)
	$(CC) src/tvmi.c -ltvm $(LFLAGS) -o $(BIN_DIR)/tvmi

tdb: libtvm $(TDB_OBJECTS) $(FREESTANDING_DEPS)
	$(CC) $(TDB_OBJECTS) -ltvm $(LFLAGS) -o $(BIN_DIR)/tdb

profile: tvmi
	time $(PROFILER) $(BIN_DIR)/tvmi $(PROF_ARGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/* $(LIB_DIR)/* libtvm/*.o tdb/*.o gmon.out *.save *.o core* vgcore*

rebuild: clean all

.PHONY : clean
.SILENT : clean
.NOTPARALLEL : clean
