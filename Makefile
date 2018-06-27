OBJ  = obj
BIN  = bin
SRC  = src
INC  = $(SRC)
LIBS = lib

# vec library
LIB_VEC_DIR = $(LIBS)/vec/src
LIB_VEC_SRC = $(LIB_VEC_DIR)/vec.c

LDFLAGS        = -Wall -lchipmunk
CFLAGS         = -std=c11 -c -Wall -Wextra -Wno-unused-parameter -I$(INC) -I$(LIB_VEC_DIR) -g
CFLAGS_DEBUG   = -O0 -DDEBUGGING
CFLAGS_RELEASE = -O2 -DNDEBUG

LIB_DIR        = libsimulator
EXE_DIR        = renderer
TEST_DIR       = tests

LIB_NAME       = libsimulator.so
EXE_NAME       = renderer
TEST_NAME      = simulator_tests

LIB            = $(BIN)/$(LIB_NAME)
EXE            = $(BIN)/$(EXE_NAME)
TEST           = $(BIN)/$(TEST_NAME)

MAKE           := $(MAKE) --no-print-directory

RELEASE ?= 0
ifeq ($(RELEASE), 1)
	CFLAGS += $(CFLAGS_RELEASE)
else
	CFLAGS += $(CFLAGS_DEBUG)
endif

LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(BIN)

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
	$(MAKE) -C $@ TARGET=../$(TEST) BIN=../$(BIN) OBJ=../$(OBJ) SRC=$@ INC=../$(LIB_DIR)/$(INC) LIBS=../$(LIB_DIR)/$(LIBS)

# helpers
.PHONY: clean run test debug test_debug build_dirs all
clean:
	rm -rf $(OBJ) $(BIN)

run: $(EXE)
	@$<

test: $(TEST)
	@$<

debug: $(EXE)
	@gdb --tui $<

test_debug: $(TEST)
	@gdb --tui $<

build_dirs:
	@mkdir -p $(BIN) $(OBJ)

all: $(LIB_DIR) $(EXE_DIR) $(TEST_DIR)
