OBJ        = obj
BIN        = bin

SRC        = src
INC        = include
LIB_INC    = $(LIB_DIR)/$(INC)

CFLAGS     = -std=c11 -c -Wall -fPIC -I$(INC) -g -O0
LDFLAGS    = -Wall

CC         = gcc
MAKE      := $(MAKE) --no-print-directory

LIB_DIR    = libsimulator
EXE_DIR    = renderer
TEST_DIR   = tests

LIB_NAME   = libsimulator.so
EXE_NAME   = renderer
TEST_NAME  = simulator_tests

LIB        = $(BIN)/$(LIB_NAME)
EXE        = $(BIN)/$(EXE_NAME)
TEST       = $(BIN)/$(TEST_NAME)

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUGGING
endif

export

.PHONY: all
all: $(LIB_DIR) $(EXE_DIR) $(TEST_DIR)

# lib
$(LIB): $(LIB_DIR)

.PHONY: $(LIB_DIR)
$(LIB_DIR): | build_dirs
	@$(MAKE) -C $@ TARGET=../$(LIB) BIN=../$(BIN) OBJ=../$(OBJ)

# renderer
$(EXE): $(EXE_DIR)

.PHONY: $(EXE_DIR)
$(EXE_DIR): $(LIB) | build_dirs
	@$(MAKE) -C $@ TARGET=../$(EXE) BIN=../$(BIN) OBJ=../$(OBJ) INC="$(INC) ../$(LIB_INC)"

# tests
$(TEST): $(TEST_DIR)

.PHONY: $(TEST_DIR)
$(TEST_DIR): $(LIB) | build_dirs
	@$(MAKE) -C $@ TARGET=../$(TEST) BIN=../$(BIN) OBJ=../$(OBJ) INC=../$(LIB_INC) SRC=.
	@LD_LIBRARY_PATH=$(BIN) $(TEST)

.PHONY: clean
clean:
	@rm -rf $(OBJ) $(BIN)

.PHONY: run
run: $(EXE)
	@LD_LIBRARY_PATH=$(BIN) $(EXE)

.PHONY: build_dirs
build_dirs:
	@mkdir -p $(BIN) $(OBJ)
