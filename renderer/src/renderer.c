#include <allegro5/allegro.h>

#include "renderer.h"
#include "graphics.h"
#include "keyboard.h"
#include "simulator/simulator.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/memory.h"
#include "util/log.h"

#define TICKS_PER_SECOND  (20)
#define FRAMES_PER_SECOND (60)

// awful and most likely temporary
//           ^^^^^^^^^^^
#define DECLARE_COLOUR(name, red, green, blue)\
	struct colour MACRO_CONCAT(COLOUR_, name) = {red/255.f, green/255.f, blue/255.f}


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

DECLARE_COLOUR(ENTITY_MALE,   105, 80, 200);
DECLARE_COLOUR(ENTITY_FEMALE, 200, 80, 105);

void step_simulation(struct renderer *renderer);
void render_simulation(struct renderer *renderer);

MODULE_IMPLEMENT(struct renderer, "renderer",
		renderer_create,
		{
			new_instance->sim = (struct simulator *)arg;

			if (!al_init())
			{
				LOG_INFO("Failed to init allegro");
				return NULL;
			}

			if ((new_instance->graphics = graphics_init(NULL)) == NULL)
			{
				LOG_INFO("Failed to init graphics");
				return NULL;
			}

			if ((new_instance->keyboard = keyboard_init(NULL)) == NULL)
			{
				LOG_INFO("Failed to init keyboard");
				return NULL;
			}

		},
		renderer_destroy,
		{
			graphics_destroy(instance->graphics);
			keyboard_destroy(instance->keyboard);
		})

void renderer_start_loop(struct renderer *renderer)
{
	// timers
	ALLEGRO_TIMER *sim_timer, *render_timer;
	if ((sim_timer = al_create_timer(1.0 / TICKS_PER_SECOND)) == NULL ||
			(render_timer = al_create_timer(1.0 / FRAMES_PER_SECOND)) == NULL)
	{
		LOG_INFO("Failed to create timers");
		return;
	}

	// events
	ALLEGRO_EVENT_QUEUE *event_queue;
	if ((event_queue = al_create_event_queue()) == NULL)
	{
		LOG_INFO("Failed to create event queue");
		return;
	}
	al_register_event_source(event_queue, graphics_get_display_event_source(renderer->graphics));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(sim_timer));
	al_register_event_source(event_queue, al_get_timer_event_source(render_timer));

	al_start_timer(sim_timer);
	al_start_timer(render_timer);

	while (TRUE)
	{
		ALLEGRO_EVENT e;
		al_wait_for_event(event_queue, &e);

		if (e.type == ALLEGRO_EVENT_TIMER)
		{
			double start_time = al_get_time();
			double total_time;
			struct time_collector *tc;

			if (e.timer.source == sim_timer)
			{
				step_simulation(renderer);
				total_time = al_get_time() - start_time;
				tc = &renderer->times.logic;

				// piggyback off simulator timer to print once per second
				if (tc->count == TICKS_PER_SECOND)
				{
					float avg_logic = tc->accumulator / tc->count;
					memset(tc, 0, sizeof(struct time_collector));

					struct time_collector *render_tc = &renderer->times.render;
					float avg_render = render_tc->accumulator / render_tc->count;
					memset(render_tc, 0, sizeof(struct time_collector));

					LOG_INFO("sim %.4f (%.2f) | render %.4f (%.2f)", avg_logic, 1./avg_logic, avg_render, 1./avg_render);
				}
			}
			else
			{
				render_simulation(renderer);
				total_time = al_get_time() - start_time;
				tc = &renderer->times.render;
			}

			tc->accumulator += total_time;
			tc->count += 1;
		}

		// quit
		else if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
				(e.type == ALLEGRO_EVENT_KEY_DOWN && e.keyboard.keycode == ALLEGRO_KEY_ESCAPE))
		{
			break;
		}

		// resize
		else if (e.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
		{
			graphics_resize(renderer->graphics, e.display.width, e.display.height);
		}

		// keyboard
		else if (e.type == ALLEGRO_EVENT_KEY_DOWN || e.type == ALLEGRO_EVENT_KEY_UP)
		{
			BOOL handled = keyboard_handle_camera_key(renderer->keyboard, e.type == ALLEGRO_EVENT_KEY_DOWN, e.keyboard.keycode);
			if (!handled)
			{
				// TODO pass off to gui/something else
			}
		}


	}

	// cleanup
	al_destroy_timer(sim_timer);
	al_destroy_timer(render_timer);
	al_destroy_event_queue(event_queue);
}

void step_simulation(struct renderer *renderer)
{
	simulator_step(renderer->sim);
}

void render_simulation(struct renderer *renderer)
{
	struct camera_movement cam;
	keyboard_get_camera_changes(renderer->keyboard, &cam);
	graphics_update_camera(renderer->graphics, cam);

	graphics_start(renderer->graphics);

	// entities
	struct entity_ctx *entity = entity_get_context(renderer->sim);
	struct component_physics *physics = (struct component_physics *)entity_get_component_array(entity, COMPONENT_PHYSICS);
	entity_id count = entity_get_count(entity);

	struct position pos;
	for (entity_id i = 0; i < count; ++i)
	{
		struct component_physics *phys = physics + i;
		world_get_position(phys->body, &pos);

		struct component_human *human = entity_get_component(entity, i, COMPONENT_HUMAN);

		graphics_draw_human(renderer->graphics, pos.x, pos.y, human->gender == MALE ? COLOUR_ENTITY_MALE : COLOUR_ENTITY_FEMALE);
	}

	graphics_end(renderer->graphics);
}
