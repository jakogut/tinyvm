CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

DEBUG = NO
PROFILE = NO

ifeq ($(DEBUG), YES)
	CFLAGS += -g
	LFLAGS += -g
endif

OBJECTS = hash_table.o instructions.o tinyvm.o memory.o program.o stack.o virtual_machine.o

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
