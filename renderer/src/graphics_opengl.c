#ifdef GRAPHICS_OPENGL

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "graphics.h"
#include "entity/components.h"

#include "util/log.h"

#define ZOOM_MAX (64.f)
#define ZOOM_MIN (0.5f)

struct colour
{
	float r, g, b;
};


// awful and most likely temporary
//           ^^^^^^^^^^^
#define DECLARE_COLOUR(red, green, blue) {red/255.f, green/255.f, blue/255.f}
#define DECLARE_COLOUR_CONSTANT(name, red, green, blue)\
	struct colour MACRO_CONCAT(COLOUR_, name) = DECLARE_COLOUR(red, green, blue)

DECLARE_COLOUR_CONSTANT(ENTITY_MALE,   105, 80 , 200);
DECLARE_COLOUR_CONSTANT(ENTITY_FEMALE, 200, 80 , 105);

struct graphics
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
		float zoom_scale;
		float x;
		float y;
	} camera;
};

static void resize(struct graphics *);

static void init_opengl(struct graphics *self)
{
	glClearColor(17.f/255, 17.f/255, 19.f/255, 1.f);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	resize(self);
}

MOD_INIT(graphics, {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if ((self->display = SDL_CreateWindow(WINDOW_TITLE,
	                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL)
	{
		LOG_ERROR("Failed to create display");
		return 1;
	}

	if ((self->gl = SDL_GL_CreateContext(self->display)) == NULL)
	{
		LOG_ERROR("Failed to create OpenGL context");
		return 1;
	}

	self->window.width = WINDOW_WIDTH;
	self->window.height = WINDOW_HEIGHT;
	self->window.aspect_ratio = 0;

	self->camera.zoom_scale = 10.f;
	self->camera.x = 0;
	self->camera.y = 0;

	init_opengl(self);
	return 0;
})

MOD_DESTROY(graphics, {
	if (self->display)
		SDL_DestroyWindow(self->display);
	if (self->gl)
		SDL_GL_DeleteContext(self->gl);
});


void graphics_start(struct graphics *self)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// position camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-self->camera.x, -self->camera.y, 0);
}

void graphics_draw_world(struct world *world)
{
	UNUSED(world);
}

void graphics_draw_human(double x, double y, struct component_human *human)
{
	// ty http://slabode.exofire.net/circle_draw.shtml
	// values hardcoded
	static double tangetial_factor = 0.3249196962329063;
	static double radial_factor    = 0.9510565162951535;
	static int segment_count       = 20;

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

void graphics_end(struct graphics *self)
{
	SDL_GL_SwapWindow(self->display);
}

static void resize(struct graphics *self)
{
	glViewport(0, 0, self->window.width, self->window.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	self->window.aspect_ratio = (double) self->window.width / self->window.height;
	glOrtho(-self->window.aspect_ratio * self->camera.zoom_scale,
			 self->window.aspect_ratio * self->camera.zoom_scale,
			 self->camera.zoom_scale,
			-self->camera.zoom_scale,
			-1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void graphics_resize(struct graphics *self, int w, int h)
{
	self->window.width = w;
	self->window.height = h;
	resize(self);
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

void graphics_update_camera(struct graphics *self, struct camera_movement changes)
{
	self->camera.x += changes.move_hor;
	self->camera.y += changes.move_ver;

	if (changes.zoom)
	{
		self->camera.zoom_scale = MIN(ZOOM_MAX, MAX(ZOOM_MIN, self->camera.zoom_scale + changes.zoom));
		zoom(self->camera.zoom_scale, self->window.aspect_ratio);
	}
}

#endif
