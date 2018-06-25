#include <SDL2/SDL.h>
#include "keyboard.h"
#include "graphics.h"

#include "util/log.h"
#include "util/memory.h"

#define MOVE_SPEED (0.45f)
#define ZOOM_SPEED (0.25f)

MOD_INIT(keyboard, {return 0;})
MOD_DESTROY(keyboard, {})

bool keyboard_handle_camera_key(struct keyboard *self, bool is_down, int key)
{
	switch(key)
	{
		case SDLK_a:
			self->camera.move_hor = is_down * -MOVE_SPEED;
			break;

		case SDLK_d:
			self->camera.move_hor = is_down * MOVE_SPEED;
			break;

		case SDLK_s:
			self->camera.move_ver = is_down * MOVE_SPEED;
			break;

		case SDLK_w:
			self->camera.move_ver = is_down * -MOVE_SPEED;
			break;

		case SDLK_KP_PLUS:
		case SDLK_PLUS:
			self->camera.zoom = is_down * -ZOOM_SPEED;
			break;

		case SDLK_KP_MINUS:
		case SDLK_MINUS:
			self->camera.zoom = is_down * ZOOM_SPEED;
			break;

		default:
			return false;
	}

	return true;
}

void keyboard_get_camera_changes(struct keyboard *self, struct camera_movement *out)
{
	*out = self->camera;
}
