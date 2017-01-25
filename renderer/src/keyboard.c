#include <SDL2/SDL.h>
#include "keyboard.h"
#include "graphics.h"

#include "util/log.h"
#include "util/memory.h"

#define MOVE_SPEED (0.45f)
#define ZOOM_SPEED (0.25f)

struct keyboard_ctx
{
	struct camera_movement camera;
};

MODULE_IMPLEMENT(struct keyboard_ctx, "keyboard",
		keyboard_init,
		{

		},
		keyboard_destroy,
		{
		})

BOOL keyboard_handle_camera_key(struct keyboard_ctx *ctx, BOOL is_down, int key)
{
	switch(key)
	{
		case SDLK_a:
			ctx->camera.move_hor = is_down * -MOVE_SPEED;
			break;

		case SDLK_d:
			ctx->camera.move_hor = is_down * MOVE_SPEED;
			break;

		case SDLK_s:
			ctx->camera.move_ver = is_down * MOVE_SPEED;
			break;

		case SDLK_w:
			ctx->camera.move_ver = is_down * -MOVE_SPEED;
			break;

		case SDLK_KP_PLUS:
		case SDLK_PLUS:
			ctx->camera.zoom = is_down * -ZOOM_SPEED;
			break;

		case SDLK_KP_MINUS:
		case SDLK_MINUS:
			ctx->camera.zoom = is_down * ZOOM_SPEED;
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

void keyboard_get_camera_changes(struct keyboard_ctx *ctx, struct camera_movement *out)
{
	*out = ctx->camera;
}
