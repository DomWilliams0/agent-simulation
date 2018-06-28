#include <SDL2/SDL.h>
#include <entity/ecs.h>

#include "renderer.h"
#include "graphics.h"
#include "keyboard.h"
#include "simulator/simulator.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/memory.h"
#include "util/log.h"

#define TICKS_PER_SECOND  (20)
#define SKIP_TICKS        (1000 / TICKS_PER_SECOND)
#define MAX_FRAMESKIP     (5)

void step_simulation(struct renderer *renderer);
void render_simulation(struct renderer *renderer, double interpolation);

MOD_INIT(renderer, {
	if (arg == NULL) {
		LOG_ERROR("Expected simulator argument to renderer");
		return 1;
	}
	self->sim = (struct simulator *) arg;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		LOG_ERROR("Failed to init SDL");
		return 1;
	}

	safe_malloc(graphics_sizeof(), &self->graphics);
	if (graphics_init(self->graphics, NULL) != 0)
		return 1;

	if (keyboard_init(&self->keyboard, NULL) != 0)
		return 1;

	return 0;
})

MOD_DESTROY(renderer, {
	if (self->graphics != NULL)
		graphics_destroy(self->graphics);
	keyboard_destroy(&self->keyboard);
	SDL_Quit();
})

#define TIME_FUNCTION(what, how) \
		uint32_t MACRO_CONCAT(what, _pre) = SDL_GetTicks(); \
		how;\
		uint32_t MACRO_CONCAT(what, _time) = SDL_GetTicks() - MACRO_CONCAT(what, _pre); \
		MACRO_CONCAT(what, _tc)->accumulator += MACRO_CONCAT(what, _time); \
		MACRO_CONCAT(what, _tc)->count += 1;

void renderer_start_loop(struct renderer *renderer)
{
	uint32_t next_tick = SDL_GetTicks();
	int loops;
	double interpolation;

	struct time_collector *logic_tc  = &renderer->times.logic;
	struct time_collector *render_tc = &renderer->times.render;

	while (true)
	{
		SDL_Event e;
		bool key_handled, key_down;
		while (SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					goto break_out;

				case SDL_KEYUP:
				case SDL_KEYDOWN:
					key_down = e.type == SDL_KEYDOWN;

					// quitting
					if (key_down && e.key.keysym.sym == SDLK_ESCAPE)
					{
						goto break_out;
					}

					// camera
					key_handled = keyboard_handle_camera_key(&renderer->keyboard, e.type == SDL_KEYDOWN, e.key.keysym.sym);
					if (!key_handled)
					{
						// TODO pass off to gui/something else
					}
                    break;

				case SDL_WINDOWEVENT:
					if (e.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						graphics_resize(renderer->graphics, e.window.data1, e.window.data2);
					}
                    break;
			}
		}

		// merci http://www.koonsolo.com/news/dewitters-gameloop/
		loops = 0;
		while (SDL_GetTicks() > next_tick && loops < MAX_FRAMESKIP)
		{
			TIME_FUNCTION(logic, step_simulation(renderer));

			next_tick += SKIP_TICKS;
			loops += 1;
		}

		interpolation = (double)(SDL_GetTicks() + SKIP_TICKS - next_tick) / SKIP_TICKS;
		interpolation /= TICKS_PER_SECOND;

		TIME_FUNCTION(render, render_simulation(renderer, interpolation));

		// print every second
		if (logic_tc->count == TICKS_PER_SECOND)
		{
			double avg_render = render_tc->accumulator / render_tc->count;
			memset(render_tc, 0, sizeof(struct time_collector));

			double avg_logic = logic_tc->accumulator / logic_tc->count;
			memset(logic_tc, 0, sizeof(struct time_collector));

			LOG_INFO("sim %.2fms (%.2f/s) | render %.2fms (%.2f/s)", avg_logic, 1000./avg_logic, avg_render, 1000./avg_render);
		}
	}

break_out:
	return;
}

void step_simulation(struct renderer *renderer)
{
	simulator_step(renderer->sim);
}

static void render_entities(struct ecs *ecs, double interpolation)
{
	ECS_COMP(physics) *physics = ecs_all(ecs, physics);
	ECS_COMP(human) *humans = ecs_all(ecs, human);

	const ecs_mask mask = ECS_COMP_MASK(physics) | ECS_COMP_MASK(human);

	for (ecs_id i = 0; i < ecs->count; ++i)
	{
		if (!ecs_has_mask(ecs, i, mask))
			continue;

		world_body body = (&physics[i])->body;
		cpVect pos = world_get_position(body);
		cpVect vel = world_get_velocity(body);

		pos = cpvadd(pos, cpvmult(vel, interpolation));
		graphics_draw_human(pos, &humans[i]);
	}
}

void render_simulation(struct renderer *renderer, double interpolation)
{
	struct graphics *graphics = renderer->graphics;

	struct camera_movement cam;
	keyboard_get_camera_changes(&renderer->keyboard, &cam);
	graphics_update_camera(graphics, cam);

	graphics_start(graphics);

	// world
	graphics_draw_world(renderer->sim->world);

	// entities
	render_entities(&renderer->sim->ecs, interpolation);

	graphics_end(renderer->graphics);
}
