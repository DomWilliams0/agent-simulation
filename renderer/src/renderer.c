#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "renderer.h"
#include "simulator/simulator.h"

#include "util/memory.h"
#include "util/log.h"
#include "util/bool.h"
#include "util/util.h"

#define TICKS_PER_SECOND  (5)
#define FRAMES_PER_SECOND (60)
#define WINDOW_WIDTH      (400)
#define WINDOW_HEIGHT     (600)


struct renderer_state
{
	struct simulator_state *sim;
	ALLEGRO_DISPLAY *window;
};

struct
{
	ALLEGRO_COLOR BG;
	ALLEGRO_COLOR TEST;
} colours;

void step_simulation(struct renderer_state *renderer);
void render_simulation(struct renderer_state *renderer);

struct renderer_state *renderer_create(struct simulator_state *sim)
{
	LOG_DEBUG("Creating new renderer");

	struct renderer_state *new_renderer;
	safe_malloc_struct(struct renderer_state, &new_renderer);

	new_renderer->sim = sim;

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

	if ((new_renderer->window = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT)) == NULL)
	{
		LOG_INFO("Failed to create display");
		return NULL;
	}

	colours.BG = al_map_rgb(17, 17, 19);
	colours.TEST = al_map_rgb(100, 190, 140);

	return new_renderer;
}

void renderer_start_loop(struct renderer_state *renderer)
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
	al_register_event_source(event_queue, al_get_display_event_source(renderer->window));
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
	al_destroy_display(renderer->window);
	al_destroy_event_queue(event_queue);

}

void renderer_destroy(struct renderer_state *renderer)
{
	safe_free(renderer);
	LOG_DEBUG("Destroyed renderer");
}

void step_simulation(struct renderer_state *renderer)
{
	UNUSED(renderer);
}

void render_simulation(struct renderer_state *renderer)
{
	UNUSED(renderer);

	al_clear_to_color(colours.BG);

	al_draw_rectangle(40, 40, 80, 100, colours.TEST, 2.f);

	al_flip_display();
}
