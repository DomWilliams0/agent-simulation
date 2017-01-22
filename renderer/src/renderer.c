#include <allegro5/allegro.h>

#include "renderer.h"
#include "graphics.h"
#include "simulator/simulator.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/memory.h"
#include "util/log.h"

#define TICKS_PER_SECOND  (20)
#define FRAMES_PER_SECOND (60)

struct renderer
{
	struct simulator *sim;
	struct graphics_ctx *graphics;
};

struct
{
	ALLEGRO_COLOR ENTITY_MALE;
	ALLEGRO_COLOR ENTITY_FEMALE;
} colours;

void step_simulation(struct renderer *renderer);
void render_simulation(struct renderer *renderer);

MODULE_IMPLEMENT(struct renderer, "renderer",
		renderer_create,
		{
			new_instance->sim = (struct simulator *)arg;

			// allegro
			if (!al_init())
			{
				LOG_INFO("Failed to init allegro");
				return NULL;
			}

			if (!al_install_keyboard())
			{
				LOG_INFO("Failed to init keyboard");
				return NULL;
			}

			if ((new_instance->graphics = graphics_init(NULL)) == NULL)
			{
				LOG_INFO("Failed to init graphics");
				return NULL;
			}

			colours.ENTITY_MALE = al_map_rgb(105, 80, 200);
			colours.ENTITY_FEMALE = al_map_rgb(200, 80, 105);
		},
		renderer_destroy,
		{
			graphics_destroy(instance->graphics);
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
			if (e.timer.source == sim_timer)
			{
				step_simulation(renderer);
			}
			else
			{
				render_simulation(renderer);
			}
		}

		// quit
		else if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
				(e.type == ALLEGRO_EVENT_KEY_DOWN && e.keyboard.keycode == ALLEGRO_KEY_ESCAPE))
		{
			break;
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

		// TODO scale view
		graphics_draw_human(renderer->graphics, pos.x, pos.y, human->gender == MALE ? &colours.ENTITY_MALE : &colours.ENTITY_FEMALE);
	}

	graphics_end(renderer->graphics);
}
