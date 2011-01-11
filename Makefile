CC = gcc
CXXFLAGS = -Wall -O3
CFLAGS = $(CXXFLAGS) -c
LFLAGS = $(CXXFLAGS)

OBJECTS = hash_table.o instructions.o tinyvm.o memory.o program.o stack.o virtual_machine.o

DEBUG = no
PROFILE = no

ifeq ($(DEBUG), yes)
	CXXFLAGS += -g
endif

ifeq ($(PROFILE), yes)
	CXXFLAGS += -pg
endif

all: tinyvm

tinyvm: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o tinyvm

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f tinyvm tinyvm-debug *.o 

rebuild: clean tinyvm

.PHONY : clean
.SILENT : clean
