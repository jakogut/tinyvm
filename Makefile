CC = gcc
AS = as

CXXFLAGS = -Wall -ansi -pedantic-errors -pipe
CFLAGS = $(CXXFLAGS) -c
LFLAGS = $(CXXFLAGS)
ASFLAGS =

SOURCES = $(wildcard *.c)
AS_SOURCES = $(SOURCES:.c=.asm)
OBJECTS = $(SOURCES:.c=.o)
AS_OBJECTS = $(SOURCES:.c=.asm.o)

BIN_NAME = tinyvm

DEBUG = no
PROFILE = no
OPTIMIZATION = -O3

ifeq ($(DEBUG), yes)
	CXXFLAGS += -g
	OPTIMIZATION = -O0
	BIN_NAME := $(BIN_NAME)-debug
endif

ifeq ($(PROFILE), yes)
	CXXFLAGS += -pg
endif

CXXFLAGS += $(OPTIMIZATION)

all: tinyvm

# Build TVM from the C sources
tinyvm: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(BIN_NAME)

# Assemble TVM from the assembly sources provided by AS_SOURCES
tinyvm-asm: $(AS_OBJECTS)
	$(CC) $(LFLAGS) $(AS_OBJECTS) -o $(BIN_NAME)

# Compile TVM to assembly
asm:	$(AS_SOURCES)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.asm.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.asm: %.c
	$(CC) -S -masm=intel $< -o $@

clean:
	rm -f $(BIN_NAME) $(BIN_NAME)-debug gmon.out *.save *.o *.asm.o *.asm core* vgcore*

rebuild: clean tinyvm

.PHONY : clean
.SILENT : clean
