OBJ        = obj
BIN        = bin
SRC        = src
INC        = $(SRC)

# TODO: move -O0 into debug flags
CFLAGS     = -std=c11 -c -Wall -Wextra -I$(INC) -g -O0
LDFLAGS    = -Wall -lchipmunk

LIB_DIR    = libsimulator
EXE_DIR    = renderer
TEST_DIR   = tests

LIB_NAME   = libsimulator.so
EXE_NAME   = renderer
TEST_NAME  = simulator_tests

LIB        = $(BIN)/$(LIB_NAME)
EXE        = $(BIN)/$(EXE_NAME)
TEST       = $(BIN)/$(TEST_NAME)

MAKE      := $(MAKE) --no-print-directory

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUGGING
endif

export

.PHONY: default $(LIB_DIR) $(EXE_DIR) $(TEST_DIR)
default: $(EXE)

# lib
$(LIB): $(LIB_DIR)
$(LIB_DIR): | build_dirs
	$(MAKE) -C $@ TARGET=../$(LIB) BIN=../$(BIN) OBJ=../$(OBJ)

# renderer
$(EXE): $(EXE_DIR)
$(EXE_DIR): $(LIB) | build_dirs
	$(MAKE) -C $@ TARGET=../$(EXE) BIN=../$(BIN) OBJ=../$(OBJ) INC=../$(LIB_DIR)/$(INC)

# tests
$(TEST): $(TEST_DIR)
$(TEST_DIR): $(LIB) | build_dirs
	$(MAKE) -C $@ TARGET=../$(TEST) BIN=../$(BIN) OBJ=../$(OBJ) SRC=$@
	@LD_LIBRARY_PATH=$(BIN) $(TEST)

# helpers
.PHONY: clean run debug build_dirs all
clean:
	rm -rf $(OBJ) $(BIN)

run: $(EXE)
	@LD_LIBRARY_PATH=$(BIN) $(EXE)

debug: $(EXE)
	@LD_LIBRARY_PATH=$(BIN) gdb --tui $(EXE)

build_dirs:
	@mkdir -p $(BIN) $(OBJ)

all: $(LIB_DIR) $(EXE_DIR) $(TEST_DIR)
