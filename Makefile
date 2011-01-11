CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

OBJECTS = hash_table.o instructions.o label.o main.o memory.o program.o stack.o virtual_machine.o

all: tinyvm

tinyvm: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o tinyvm

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f tinyvm *.o 

rebuild: clean tinyvm

.PHONY : clean
.SILENT : clean
