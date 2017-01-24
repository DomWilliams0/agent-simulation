#include <allegro5/allegro.h>
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
			if (!al_install_keyboard())
			{
				LOG_ERROR("Failed to install keyboard");
				MODULE_INIT_ABORT;
			}

		},
		keyboard_destroy,
		{
			al_uninstall_keyboard();
		})


BOOL keyboard_handle_camera_key(struct keyboard_ctx *ctx, BOOL is_down, int key)
{
	switch(key)
	{
		case ALLEGRO_KEY_A:
			ctx->camera.move_hor = is_down * -MOVE_SPEED;
			break;

		case ALLEGRO_KEY_D:
			ctx->camera.move_hor = is_down * MOVE_SPEED;
			break;

		case ALLEGRO_KEY_S:
			ctx->camera.move_ver = is_down * MOVE_SPEED;
			break;

		case ALLEGRO_KEY_W:
			ctx->camera.move_ver = is_down * -MOVE_SPEED;
			break;

		case ALLEGRO_KEY_PAD_PLUS:
			ctx->camera.zoom = is_down * -ZOOM_SPEED;
			break;

		case ALLEGRO_KEY_PAD_MINUS:
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
