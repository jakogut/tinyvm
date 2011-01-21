CC = gcc
CXXFLAGS = -Wall -O3
CFLAGS = $(CXXFLAGS) -c
LFLAGS = $(CXXFLAGS)

OBJECTS = hash_table.o tinyvm.o memory.o program.o stack.o virtual_machine.o

DEBUG = no
PROFILE = no

BIN_NAME = tinyvm

ifeq ($(DEBUG), yes)
	CXXFLAGS += -g
	BIN_NAME := $(BIN_NAME)-debug
endif

ifeq ($(PROFILE), yes)
	CXXFLAGS += -pg
endif

all: tinyvm

tinyvm: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(BIN_NAME)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN_NAME) $(BIN_NAME)-debug gmon.out *.save *.o

rebuild: clean tinyvm

.PHONY : clean
.SILENT : clean
