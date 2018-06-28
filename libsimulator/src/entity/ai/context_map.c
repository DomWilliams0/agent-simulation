#include <math.h>
#include <string.h>
#include <assert.h>

#include "context_map.h"

void cm_reset(struct context_map *map)
{
	memset(map, 0, sizeof(struct context_map));
}

void cm_add(struct context_map *map,
            enum cm_type type,
            enum cm_direction direction,
            float weight)
{
	assert(!isinf(weight) && !isnan(weight));
	cm_slots *slots = &map->maps[type];
	(*slots)[direction] += weight;

	// falloff
	const float falloff_factor = 0.25;

	float falloff = falloff_factor * weight;
	(*slots)[(direction - 1) % CM_DIRECTION_COUNT] += falloff;
	(*slots)[(direction + 1) % CM_DIRECTION_COUNT] += falloff;
}

static float find_min(cm_slots *slots)
{
	float current = 1.0;
	for (int i = 0; i < CM_DIRECTION_COUNT; ++i)
	{
		if ((*slots)[i] < current)
			current = (*slots)[i];
	}
	return current;
}

static enum cm_direction find_max_index(cm_slots *slots)
{
	float current = 0.0;
	enum cm_direction current_i = CM_DIRECTION_COUNT;
	for (int i = 0; i < CM_DIRECTION_COUNT; ++i)
	{
		if ((*slots)[i] > current)
		{
			current = (*slots)[i];
			current_i = (enum cm_direction) i;
		}
	}
	return current_i;
}

static void mask_danger(cm_slots *interest,
                        cm_slots *danger,
                        float min_danger)
{
	for (int i = 0; i < CM_DIRECTION_COUNT; ++i)
	{
		float d = (*danger)[i];
		if (d > min_danger)
			(*interest)[i] = 0.0;
	}

}

static const float ANGLE_DIV = 0.7853981634f; // 45 degrees

float cm_direction_angle(enum cm_direction direction)
{
	return direction * ANGLE_DIV;
}

enum cm_direction cm_direction_from_angle(float angle)
{
	int snapped = (int) roundf(angle / ANGLE_DIV);
	return (enum cm_direction) snapped % CM_DIRECTION_COUNT; // TODO avoid expensive mod
}

enum cm_direction cm_calculate(struct context_map *map, float *force_out)
{
	cm_slots *interest = &map->maps[CM_INTEREST];
	cm_slots *danger = &map->maps[CM_DANGER];

	// find lowest danger
	float min_danger = find_min(danger);

	// zero all slots with non-min danger in interest map
	mask_danger(interest, danger, min_danger);

	// find max of interest
	enum cm_direction best_direction = find_max_index(interest);
	float force = (float) (best_direction == CM_DIRECTION_COUNT ? 0.0 : (*interest)[best_direction]);

	assert(!isinf(force) && !isnan(force));

	// TODO calculate gradient around this point for smoothing
	*force_out = force;
	return best_direction;
}
