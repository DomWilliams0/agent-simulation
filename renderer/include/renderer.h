#ifndef RENDERER_H
#define RENDERER_H

#include "util/util.h"
#include "keyboard.h"

struct time_collector
{
	double accumulator;
	int count;
};

MOD_DECLARE(renderer, {
	struct simulator *sim;
	struct graphics *graphics;
	struct keyboard keyboard;

	struct
	{
		struct time_collector logic;
		struct time_collector render;
		struct time_collector frame;
		double last_frame;
	} times;
})


void renderer_start_loop(struct renderer *renderer);

#endif
