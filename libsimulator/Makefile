LIB   = lib
CFLAGS += -I$(LIB)

SRCS  := $(shell find $(SRC) -type f -name '*.c')
LIBS  := $(shell find $(LIB) -type f -name '*.c')

OBJS  := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))
OBJS  += $(addprefix $(OBJ)/,$(notdir $(LIBS:%.c=%.o)))

VPATH  = $(shell find $(SRC) $(INC) $(LIB) -type d)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ -shared
	@echo "Compiled $(TARGET)"

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
