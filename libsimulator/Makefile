SRCS  := $(shell find $(SRC) -type f -name '*.c')
OBJS  := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))
VPATH  = $(shell find $(SRC) $(INC) -type d)

CFLAGS += -fPIC

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@ -shared
	@echo "Compiled $(TARGET)"

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@