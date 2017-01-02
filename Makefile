OBJ = obj
SRC = src
BIN = bin
INC = include
SRC_TEST = test

CFLAGS = -std=c11 -c -Wall -I$(INC) -fPIC
LFLAGS = -Wall

CFLAGS_TEST = $(CFLAGS)
LFLAGS_TEST = $(LFLAGS) -lcmocka

CC = gcc
TARGET = simulator
TARGET_LIB = lib$(TARGET).so
TARGET_TESTS = tests_$(TARGET)

BIN_PATH=$(BIN)/$(TARGET)
BIN_PATH_TESTS=$(BIN)/$(TARGET_TESTS)

SRCS := $(shell find $(SRC) -type f -name '*.c')
INCS := $(shell find $(INC) -type f -name '*.h')
OBJS := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))

SRCS_TEST := $(shell find $(SRC_TEST) -type f -name '*.c')
OBJS_TEST := $(SRCS_TEST:$(SRC_TEST)/%.c=$(OBJ)/%.o)

VPATH = $(shell find $(SRC) $(INC) -type d)

# main executable
$(BIN_PATH): $(OBJS) | build_dirs
	$(CC) $(LFLAGS) $(OBJS) -o $@

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c | build_dirs
	$(CC) $(CFLAGS) -c $< -o $@

# shared lib
$(BIN)/$(TARGET_LIB): $(OBJS) | build_dirs
	$(CC) $(LFLAGS) $(OBJS) -o $@ -shared

# test executable
$(BIN_PATH_TESTS): $(BIN)/$(TARGET_LIB) $(OBJS_TEST) | build_dirs
	$(CC) $(LFLAGS_TEST) $(OBJS_TEST) -o $@ -L$(BIN) -l$(TARGET)

# test src -> obj
$(OBJS_TEST): $(OBJ)/%.o : $(SRC_TEST)/%.c | build_dirs
	$(CC) $(CFLAGS) -c $< -o $@

build_dirs:
	@mkdir -p $(BIN) $(OBJ)

.PHONY: clean
clean:
	@rm -rf $(OBJ) $(BIN)

.PHONY: build
build: $(BIN_PATH)

.PHONY: tests
tests: $(BIN_PATH_TESTS)
	@LD_LIBRARY_PATH=$(BIN) $(BIN_PATH_TESTS)

.PHONY: run
run: $(BIN_PATH)
	@$(BIN_PATH)
