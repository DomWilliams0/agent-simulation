OBJ = obj
SRC = src
BIN = bin
INC = include

CFLAGS = -std=c11 -c -Wall -I$(INC)
LFLAGS = -Wall

CC = gcc
TARGET = simulator

SRCS := $(shell find $(SRC) -type f -name '*.c')
INCS := $(shell find $(INC) -type f -name '*.h')
OBJS := $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)

$(BIN)/$(TARGET): $(OBJS) | build_dirs
	$(CC) $(LFLAGS) $(OBJS) -o $@

$(OBJS): $(OBJ)/%.o : $(SRC)/%.c | build_dirs
	$(CC) $(CFLAGS) -c $< -o $@

build_dirs:
	@mkdir -p $(BIN) $(OBJ)

.PHONY: clean
clean:
	@rm -rf $(OBJ) $(BIN)
