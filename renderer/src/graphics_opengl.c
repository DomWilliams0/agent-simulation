#ifdef GRAPHICS_OPENGL

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

#include "graphics.h"

#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"
#include "util/constants.h"

#define ZOOM_MAX (16.0f)
#define ZOOM_MIN (0.5f)

struct graphics_ctx
{
	ALLEGRO_DISPLAY *display;

	struct
	{
		int width;
		int height;
		float aspect_ratio;
	} window;

	struct
	{
		float zoom_scale;
		float x;
		float y;
	} camera;
};

static void resize(struct graphics_ctx *);

static void init_opengl(struct graphics_ctx *ctx)
{
	glClearColor(17.f/255, 17.f/255, 19.f/255, 1.f);
	glDisable(GL_DEPTH_TEST);

	resize(ctx);
}

MODULE_IMPLEMENT(struct graphics_ctx, "OpenGL graphics context",
		graphics_init,
		{
			al_set_new_display_flags(ALLEGRO_OPENGL);
			if ((new_instance->display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT)) == NULL)
			{
				LOG_INFO("Failed to create display");
				return NULL;
			}

			new_instance->window.width = WINDOW_WIDTH;
			new_instance->window.height = WINDOW_HEIGHT;
			new_instance->window.aspect_ratio = 0;

			new_instance->camera.zoom_scale = 10.f;
			new_instance->camera.x = 50;
			new_instance->camera.y = 50;

			init_opengl(new_instance);

		},
		graphics_destroy,
		{
		})

ALLEGRO_EVENT_SOURCE *graphics_get_display_event_source(struct graphics_ctx *ctx)
{
	return al_get_display_event_source(ctx->display);
}

void graphics_start(struct graphics_ctx *ctx)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// position camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-ctx->camera.x, -ctx->camera.y, 0);
}

void graphics_draw_human(struct graphics_ctx *ctx, float x, float y, struct colour colour)
{
	UNUSED(ctx);

	glPushMatrix();
	glColor3fv((GLfloat *)&colour);

	glTranslatef(x, y, 0);

	glBegin(GL_QUADS);

	glVertex2f(0, 0);
	glVertex2f(0, HUMAN_RADIUS);
	glVertex2f(HUMAN_RADIUS, HUMAN_RADIUS);
	glVertex2f(HUMAN_RADIUS, 0);

	glEnd();

	glPopMatrix();
}

void graphics_end(struct graphics_ctx *ctx)
{
	UNUSED(ctx);

	al_flip_display();
}

static void resize(struct graphics_ctx *ctx)
{
	glViewport(0, 0, ctx->window.width, ctx->window.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ctx->window.aspect_ratio = (float)ctx->window.width / ctx->window.height;
	glOrtho(-ctx->window.aspect_ratio * ctx->camera.zoom_scale,
			 ctx->window.aspect_ratio * ctx->camera.zoom_scale,
			 ctx->camera.zoom_scale,
			-ctx->camera.zoom_scale,
			-1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void graphics_resize(struct graphics_ctx *ctx, int w, int h)
{
	ctx->window.width = w;
	ctx->window.height = h;
	resize(ctx);
}

static void zoom(float scale, float aspect_ratio)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-aspect_ratio * scale,
			 aspect_ratio * scale,
			 scale,
			-scale,
			-1, 1);
}

void graphics_update_camera(struct graphics_ctx *ctx, struct camera_movement changes)
{
	ctx->camera.x += changes.move_hor;
	ctx->camera.y += changes.move_ver;

	if (changes.zoom)
	{
		ctx->camera.zoom_scale = MIN(ZOOM_MAX, MAX(ZOOM_MIN, ctx->camera.zoom_scale + changes.zoom));
		zoom(ctx->camera.zoom_scale, ctx->window.aspect_ratio);
	}
}

#endif
