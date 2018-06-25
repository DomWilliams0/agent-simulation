SRCS  := $(shell find $(SRC) -type f -name '*.c')
OBJS  := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))
VPATH  = $(shell find $(SRC) $(INC) -type d)

$(TARGET): $(OBJS)
	@echo objs ${OBJS}
	$(AR) rcs $@ $^
	@echo "Compiled $(TARGET)"

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@