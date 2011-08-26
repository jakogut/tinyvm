CC = gcc
AS = as

CXXFLAGS = -Wall -pipe -Iinclude/
CFLAGS = $(CXXFLAGS) -c
LFLAGS = $(CXXFLAGS) -L lib/
ASFLAGS =
PEDANTIC_FLAGS = -ansi -pedantic -pedantic-errors

LIBTVM_SOURCES = $(wildcard libtvm/*.c)
LIBTVM_OBJECTS = $(LIBTVM_SOURCES:.c=.o)

BIN_DIR = bin
LIB_DIR = lib
INC_DIR = include
PROGRAM_DIR = programs

DEBUG = no
PROFILE = no
PEDANTIC = no
OPTIMIZATION = -O3

ifeq ($(DEBUG), yes)
	CXXFLAGS += -g
	OPTIMIZATION = -O0
endif

ifeq ($(PROFILE), yes)
	CXXFLAGS += -pg
endif

ifeq ($(PEDANTIC), yes)
	CXXFLAGS += $(PEDANTIC_FLAGS)
endif

CXXFLAGS += $(OPTIMIZATION)

all: libtvm tvmi

install: libtvm tvmi
	cp -f bin/tvmi /usr/bin/
	cp -rf include/tvm /usr/include/
	cp -f lib/libtvm* /usr/lib/

uninstall:
	rm -rf /usr/bin/tvmi
	rm -rf /usr/include/tvmi
	rm -rf /usr/lib/libtvm*

libtvm: $(LIBTVM_OBJECTS)
	mkdir -p $(LIB_DIR)
	ar rcs $(LIB_DIR)/libtvm.a $(LIBTVM_OBJECTS)

# Build the TVM interpreter
tvmi: libtvm
	mkdir -p $(BIN_DIR)
	$(CC) $(LFLAGS) tvmi.c -ltvm -o $(BIN_DIR)/tvmi

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)/* $(LIB_DIR)/*  libtvm/*.o gmon.out *.save *.o core* vgcore*

rebuild: clean all

.PHONY : clean
.SILENT : clean
