#ifdef GRAPHICS_OPENGL

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "graphics.h"
#include "world/world.h"
#include "entity/components.h"

#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"
#include "util/constants.h"

#define ZOOM_MAX (64.f)
#define ZOOM_MIN (0.5f)

// awful and most likely temporary
//           ^^^^^^^^^^^
#define DECLARE_COLOUR(red, green, blue) {red/255.f, green/255.f, blue/255.f}
#define DECLARE_COLOUR_CONSTANT(name, red, green, blue)\
	struct colour MACRO_CONCAT(COLOUR_, name) = DECLARE_COLOUR(red, green, blue)

DECLARE_COLOUR_CONSTANT(ENTITY_MALE,   105, 80 , 200);
DECLARE_COLOUR_CONSTANT(ENTITY_FEMALE, 200, 80 , 105);

static struct colour TILE_COLOURS[] = {
	DECLARE_COLOUR(40, 40,  48), // TILE_BLANK
	DECLARE_COLOUR(50, 140, 80), // TILE_GRASS
};

struct graphics_ctx
{
	SDL_Window *display;
	SDL_GLContext gl;

	struct
	{
		int width;
		int height;
		double aspect_ratio;
	} window;

	struct
	{
		double zoom_scale;
		double x;
		double y;
	} camera;
};

static void resize(struct graphics_ctx *);

static void init_opengl(struct graphics_ctx *ctx)
{
	glClearColor(17.f/255, 17.f/255, 19.f/255, 1.f);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	resize(ctx);
}

MODULE_IMPLEMENT(struct graphics_ctx, "OpenGL graphics context",
		graphics_init,
		{
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			if ((new_instance->display = SDL_CreateWindow(WINDOW_TITLE,
							SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL)
			{
				LOG_ERROR("Failed to create display");
				MODULE_INIT_ABORT;
			}

			if ((new_instance->gl = SDL_GL_CreateContext(new_instance->display)) == NULL)
			{
				LOG_ERROR("Failed to create OpenGL context");
				MODULE_INIT_ABORT;
			}

			new_instance->window.width = WINDOW_WIDTH;
			new_instance->window.height = WINDOW_HEIGHT;
			new_instance->window.aspect_ratio = 0;

			new_instance->camera.zoom_scale = 10.f;
			new_instance->camera.x = 0;
			new_instance->camera.y = 0;

			init_opengl(new_instance);

		},
		graphics_destroy,
		{
			if (instance->display)
				SDL_DestroyWindow(instance->display);
			if (instance->gl)
				SDL_GL_DeleteContext(instance->gl);
		})


void graphics_start(struct graphics_ctx *ctx)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// position camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-ctx->camera.x, -ctx->camera.y, 0);
}

void graphics_draw_world(struct world *world)
{
	const chunk_coord width = world_get_chunk_width(world);

	chunk_coord chunk_count;
	struct chunk * chunk = world_get_chunk_array(world, &chunk_count);

	glPushMatrix();

	for (chunk_coord chunk_i = 0; chunk_i < chunk_count; ++chunk_i)
	{
		tile *tile = world_get_chunk_tiles(chunk);

		glBegin(GL_QUADS);
		for (tile_coord t = 0; t < CHUNK_TILE_COUNT; ++t)
		{
			struct colour *colour = TILE_COLOURS + *tile;
			glColor3fv((GLfloat *)colour);

			tile_coord tile_x = t % CHUNK_SIZE;
			tile_coord tile_y = t / CHUNK_SIZE;

			glVertex2f(tile_x + 0, tile_y + 0);
			glVertex2f(tile_x + 0, tile_y + 1);
			glVertex2f(tile_x + 1, tile_y + 1);
			glVertex2f(tile_x + 1, tile_y + 0);

			tile++;
		}
		glEnd();

		if ((chunk_i + 1) % width == 0)
		{
			// new row
			int col_reset = -(width - 1) * CHUNK_SIZE;
			int row_shift = CHUNK_SIZE;
			glTranslatef(col_reset, row_shift, 0);
		}
		else
		{
			// move along
			glTranslatef(CHUNK_SIZE, 0, 0);
		}

		chunk++;
	}

#ifdef DEBUGGING_OVERLAY
	const chunk_coord height = world_get_chunk_height(world);

	glTranslatef(0, -CHUNK_SIZE * (float)height, 0);
	glBegin(GL_LINES);

	const tile_coord line_hor_count = (height * CHUNK_SIZE) + 1;
	const tile_coord line_ver_count = (width * CHUNK_SIZE) + 1;

	// tile outlines
	glColor3f(0.75f, 0.1f, 0.1f);
	for (tile_coord line_hor = 0; line_hor < line_hor_count; ++line_hor)
	{
		glVertex2f(0, line_hor);
		glVertex2f(width * CHUNK_SIZE, line_hor);
	}

	for (tile_coord line_ver = 0; line_ver < line_ver_count; ++line_ver)
	{
		glVertex2f(line_ver, 0);
		glVertex2f(line_ver, height * CHUNK_SIZE);
	}

	// chunk outlines
	const chunk_coord chunk_line_hor_count = height + 1;
	const chunk_coord chunk_line_ver_count = width + 1;

	glColor3f(0.5f, 0.2f, 0.5f);
	for (tile_coord line_hor = 0; line_hor < chunk_line_hor_count; ++line_hor)
	{
		glVertex2f(0, line_hor * CHUNK_SIZE);
		glVertex2f(width * CHUNK_SIZE, line_hor * CHUNK_SIZE);
	}

	for (tile_coord line_ver = 0; line_ver < chunk_line_ver_count; ++line_ver)
	{
		glVertex2f(line_ver * CHUNK_SIZE, 0);
		glVertex2f(line_ver * CHUNK_SIZE, height * CHUNK_SIZE);
	}
	glEnd();
#endif

	glPopMatrix();

}

void graphics_draw_human(double x, double y, struct component_human *human)
{
	// ty http://slabode.exofire.net/circle_draw.shtml
	// values hardcoded
	static double tangetial_factor = 0.3249196962329063;
	static double radial_factor    = 0.9510565162951535;
	static int segment_count      = 20;

	glPushMatrix();
	if (human->gender == MALE)
		glColor3fv((GLfloat *)&COLOUR_ENTITY_MALE);
	else
		glColor3fv((GLfloat *)&COLOUR_ENTITY_FEMALE);

	glTranslatef((GLfloat) x, (GLfloat) y, 0);

	double cx = HUMAN_RADIUS;
	double cy = 0;

	glBegin(GL_POLYGON);
	for(int ii = 0; ii < segment_count; ++ii)
	{
		glVertex2f((GLfloat) cx, (GLfloat) cy);

		double tx = -cy;
		double ty = cx;

		cx += tx * tangetial_factor;
		cy += ty * tangetial_factor;

		cx *= radial_factor;
		cy *= radial_factor;
	}
	glEnd();

	glPopMatrix();
}

void graphics_end(struct graphics_ctx *ctx)
{
	SDL_GL_SwapWindow(ctx->display);
}

static void resize(struct graphics_ctx *ctx)
{
	glViewport(0, 0, ctx->window.width, ctx->window.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ctx->window.aspect_ratio = (double) ctx->window.width / ctx->window.height;
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

static void zoom(double scale, double aspect_ratio)
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
