CFLAGS  += $(addprefix -I,$(INC))
LDFLAGS += -lode -lsimulator -L$(BIN)

SRCS    := $(shell find $(SRC) -type f -name '*.c')
INCS    := $(shell find $(INC) -type f -name '*.h')
OBJS    := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))

VPATH    = $(shell find $(SRC) $(INC) -type d)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	@echo "Compiled $(TARGET)"

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
