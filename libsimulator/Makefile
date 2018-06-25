SRCS  := $(shell find $(SRC) -type f -name '*.c')
OBJS  := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))
VPATH  = $(shell find $(SRC) $(INC) -type d)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ -shared
	@echo "Compiled $(TARGET)"

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
