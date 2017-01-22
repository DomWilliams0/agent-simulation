#ifndef RENDERER_H
#define RENDERER_H

#include "util/util.h"

MODULE_DECLARE(struct renderer_state, renderer_create, renderer_destroy)

void renderer_start_loop(struct renderer_state *renderer);

#endif
