#include <SDL2/SDL.h>

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

struct time_collector
{
	double accumulator;
	int count;
};

struct renderer
{
	struct simulator *sim;
	struct graphics_ctx *graphics;
	struct keyboard_ctx *keyboard;

	struct
	{
		struct time_collector logic;
		struct time_collector render;
		struct time_collector frame;
		float last_frame;
	} times;
};

void step_simulation(struct renderer *renderer);
void render_simulation(struct renderer *renderer);

MODULE_IMPLEMENT(struct renderer, "renderer",
		renderer_create,
		{
			new_instance->sim = (struct simulator *)arg;

			if (SDL_Init(SDL_INIT_VIDEO))
			{
				LOG_ERROR("Failed to init SDL");
				MODULE_INIT_ABORT;
			}

			if ((new_instance->graphics = graphics_init(NULL)) == NULL)
			{
				MODULE_INIT_ABORT;
			}

			if ((new_instance->keyboard = keyboard_init(NULL)) == NULL)
			{
				MODULE_INIT_ABORT;
			}

		},
		renderer_destroy,
		{
			if (instance->graphics)
				graphics_destroy(instance->graphics);
			if (instance->keyboard)
				keyboard_destroy(instance->keyboard);

			SDL_Quit();
		})

#define TIME_FUNCTION(what, how) \
		unsigned int MACRO_CONCAT(what, _pre) = SDL_GetTicks(); \
		how;\
		unsigned int MACRO_CONCAT(what, _time) = SDL_GetTicks() - MACRO_CONCAT(what, _pre); \
		MACRO_CONCAT(what, _tc)->accumulator += MACRO_CONCAT(what, _time); \
		MACRO_CONCAT(what, _tc)->count += 1;

void renderer_start_loop(struct renderer *renderer)
{
	unsigned int next_tick = SDL_GetTicks();
	int loops;
	float interpolation;

	struct time_collector *logic_tc  = &renderer->times.logic;
	struct time_collector *render_tc = &renderer->times.render;

	while (TRUE)
	{
		SDL_Event e;
		BOOL key_handled, key_down;
		while (SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					goto break_out;
					break;

				case SDL_KEYUP:
				case SDL_KEYDOWN:
					key_down = e.type == SDL_KEYDOWN;

					// quitting
					if (key_down && e.key.keysym.sym == SDLK_ESCAPE)
					{
						goto break_out;
					}

					// camera
					key_handled = keyboard_handle_camera_key(renderer->keyboard, e.type == SDL_KEYDOWN, e.key.keysym.sym);
					if (!key_handled)
					{
						// TODO pass off to gui/something else
					}

				case SDL_WINDOWEVENT:
					if (e.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						graphics_resize(renderer->graphics, e.window.data1, e.window.data2);
					}
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

		interpolation = (float)(SDL_GetTicks() + SKIP_TICKS - next_tick) / SKIP_TICKS;

		TIME_FUNCTION(render, render_simulation(renderer));

		// print every second
		if (logic_tc->count == TICKS_PER_SECOND)
		{
			float avg_render = render_tc->accumulator / render_tc->count;
			memset(render_tc, 0, sizeof(struct time_collector));

			float avg_logic = logic_tc->accumulator / logic_tc->count;
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

static void render_entities(struct entity_ctx *entities)
{
	struct component_physics *physics = (struct component_physics *)entity_get_component_array(entities, COMPONENT_PHYSICS);
	struct component_human *humans = (struct component_human *)entity_get_component_array(entities, COMPONENT_HUMAN);

	entity_id count = entity_get_count(entities);
	const entity_mask render_mask = COMPONENT_PHYSICS | COMPONENT_HUMAN;

	struct position pos;
	for (entity_id i = 0; i < count; ++i)
	{
		if (!entity_has_component(entities, i, render_mask))
			continue;

		struct component_physics *phys = physics + i;
		world_get_position(phys->body, &pos);

		struct component_human *human = humans + i;
		graphics_draw_human(pos.x, pos.y, human);
	}
}

void render_simulation(struct renderer *renderer)
{
	struct graphics_ctx *graphics = renderer->graphics;

	struct camera_movement cam;
	keyboard_get_camera_changes(renderer->keyboard, &cam);
	graphics_update_camera(graphics, cam);

	graphics_start(graphics);

	// world
	struct world *world = world_get_world(renderer->sim);
	graphics_draw_world(world);

	// entities
	struct entity_ctx *entities = entity_get_context(renderer->sim);
	render_entities(entities);

	graphics_end(renderer->graphics);
}
