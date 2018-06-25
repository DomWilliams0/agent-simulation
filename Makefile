OBJ        = obj
BIN        = bin
SRC        = src
INC        = include

# TODO: move -O0 into debug flags
CFLAGS     = -std=c11 -c -Wall -Wextra -I$(INC) -g -O0
LDFLAGS    = -Wall

LIB_DIR    = libsimulator
EXE_DIR    = renderer
TEST_DIR   = tests

LIB_NAME   = libsimulator.a
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

.PHONY: default, sim, render, test
default: $(EXE)

# lib
sim: $(LIB)
$(LIB): $(LIB_DIR) | build_dirs
	$(MAKE) -C $< TARGET=../$@ BIN=../$(BIN) OBJ=../$(OBJ)

# renderer
render: $(EXE)
$(EXE): $(EXE_DIR) $(LIB) | build_dirs
	$(MAKE) -C $< TARGET=../$@ BIN=../$(BIN) OBJ=../$(OBJ) INC=../$(LIB_DIR)/$(INC)

# tests
test: $(TEST)
$(TEST): $(TEST_DIR) | build_dirs
	$(MAKE) -C $< TARGET=../$@ BIN=../$(BIN) OBJ=../$(OBJ) SRC=. INC=../$(LIB_DIR)/$(INC)
	@$@

# helpers
.PHONY: clean, run, debug, build_dirs, all
clean:
	rm -rf $(OBJ) $(BIN)

run: $(EXE)
	@$(EXE)

debug: $(EXE)
	@gdb --tui $(EXE)

build_dirs:
	@mkdir -p $(BIN) $(OBJ)

all: $(LIB_DIR) $(EXE_DIR) $(TEST_DIR)
