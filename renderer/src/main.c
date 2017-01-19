#include "renderer.h"

int main()
{
	struct renderer_state *renderer;

	if (!renderer_init(&renderer))
		return 1;

	renderer_start_loop(renderer);

	renderer_destroy(renderer);

	return 0;
}

