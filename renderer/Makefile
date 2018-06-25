# renderer choice
RENDER_BACKEND ?= hardware
ifeq ($(RENDER_BACKEND),hardware)
    RENDERER = GRAPHICS_OPENGL
else ifeq ($(RENDER_BACKEND),terminal)
    RENDERER = GRAPHICS_TERMINAL
else
    $(error RENDER_BACKEND must be either 'hardware' or 'terminal')
endif

CFLAGS  += $(addprefix -I,$(INC)) -D$(RENDERER) -DDEBUGGING_OVERLAY
LDFLAGS += -lode -lsimulator -lGL -lSDL2 -lm -L$(BIN)

SRCS    := $(shell find $(SRC) -type f -name '*.c')
INCS    := $(shell find $(INC) -type f -name '*.h')
OBJS    := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.c=%.o)))

VPATH    = $(shell find $(SRC) $(INC) -type d)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "Compiled $(TARGET)"

# src -> obj
$(OBJS): $(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
