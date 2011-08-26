CC = gcc
AS = as
MKDIR = mkdir

CXXFLAGS = -Wall -pipe
CFLAGS = $(CXXFLAGS) -c
LFLAGS = $(CXXFLAGS)
ASFLAGS =
PEDANTIC_FLAGS = -ansi -pedantic -pedantic-errors

SOURCES = $(wildcard *.c)
AS_SOURCES = $(SOURCES:.c=.asm)
OBJECTS = $(SOURCES:.c=.o)
AS_OBJECTS = $(SOURCES:.c=.asm.o)

PROGRAM_DIR = programs
BIN_DIR = bin
BIN_NAME = tinyvm

DEBUG = no
PROFILE = no
PEDANTIC = no
OPTIMIZATION = -O3

ifeq ($(DEBUG), yes)
	CXXFLAGS += -g
	OPTIMIZATION = -O0
	BIN_NAME := $(BIN_NAME)-debug
endif

ifeq ($(PROFILE), yes)
	CXXFLAGS += -pg
	BIN_NAME := $(BIN_NAME)
endif

ifeq ($(PEDANTIC), yes)
	CXXFLAGS += $(PEDANTIC_FLAGS)
endif

CXXFLAGS += $(OPTIMIZATION)

all: $(BIN_DIR)/$(BIN_NAME)

# Build TVM from the C sources
$(BIN_DIR)/$(BIN_NAME): $(OBJECTS) $(BIN_DIR)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(BIN_DIR)/$(BIN_NAME)
	ln -s ../$(BIN_DIR)/$(BIN_NAME) ./$(PROGRAM_DIR)

# Make the bin dir if it's missing
$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

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
	rm -f $(BIN_DIR)/* $(PROGRAM_DIR)/$(BIN_NAME)* gmon.out *.save *.o *.asm.o *.asm core* vgcore*

rebuild: clean all

.PHONY : clean
.SILENT : clean
