ALLEGRO_LIBS = main primitives color
ALLEGRO_LIBS := -lallegro $(addprefix -lallegro_,$(ALLEGRO_LIBS))

# renderer choice
RENDER_BACKEND ?= hardware
ifeq ($(RENDER_BACKEND),software)
    RENDERER = GRAPHICS_ALLEGRO
else ifeq ($(RENDER_BACKEND),hardware)
    RENDERER = GRAPHICS_OPENGL
else
    $(error RENDER_BACKEND must be either 'software' or 'hardware')
endif

CFLAGS  += $(addprefix -I,$(INC)) -D$(RENDERER)
LDFLAGS += -lode -lsimulator $(ALLEGRO_LIBS) -L$(BIN)

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
