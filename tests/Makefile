CFLAGS  += $(addprefix -I,$(INC))
LDFLAGS += -lsimulator -pthread -L$(BIN)

SRCS    := $(shell find . -type f -name '*.c')
OBJS    := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "Compiled $(TARGET)"

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
