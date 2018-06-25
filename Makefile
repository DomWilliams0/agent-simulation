OBJ        = obj
BIN        = bin
SRC        = src
INC        = $(SRC)

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

.PHONY: default sim render test
default: render

# lib
sim: | build_dirs
	$(MAKE) -C $(LIB_DIR) TARGET=../$(LIB) BIN=../$(BIN) OBJ=../$(OBJ)

# renderer
render: sim | build_dirs
	$(MAKE) -C $(EXE_DIR) TARGET=../$(EXE) BIN=../$(BIN) OBJ=../$(OBJ) INC=../$(LIB_DIR)/$(INC)

# tests
test: | build_dirs
	$(MAKE) -C $(TEST_DIR) TARGET=../$(TEST) BIN=../$(BIN) OBJ=../$(OBJ) SRC=. INC=../$(LIB_DIR)/$(INC)
	@$@

# helpers
.PHONY: clean run debug build_dirs all
clean:
	rm -rf $(OBJ) $(BIN)

run: $(EXE)
	@$(EXE)

debug: $(EXE)
	@gdb --tui $(EXE)

build_dirs:
	@mkdir -p $(BIN) $(OBJ)

all: $(LIB_DIR) $(EXE_DIR) $(TEST_DIR)
