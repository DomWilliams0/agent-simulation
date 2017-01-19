#ifndef RENDERER_H
#define RENDERER_H

#include "util/bool.h"

struct renderer_state;

struct renderer_state *renderer_create();

void renderer_start_loop(struct renderer_state *renderer);

void renderer_destroy(struct renderer_state *renderer);

#endif
