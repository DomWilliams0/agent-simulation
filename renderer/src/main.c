#include <stddef.h>

#include "renderer.h"

int main()
{
	struct renderer_state *renderer;

	if ((renderer = renderer_create()) == NULL)
		return 1;

	renderer_start_loop(renderer);

	renderer_destroy(renderer);

	return 0;
}

