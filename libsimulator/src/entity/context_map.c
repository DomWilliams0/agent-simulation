#include <string.h>

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
		if (*slots[i] < current)
			current = *slots[i];
	}
	return current;
}

static float find_max(cm_slots *slots)
{
	float current = 0.0;
	for (int i = 0; i < CM_DIRECTION_COUNT; ++i)
	{
		if (*slots[i] > current)
			current = *slots[i];
	}
	return current;
}

static void mask_danger(cm_slots *interest,
                        cm_slots *danger,
                        float min_danger)
{
	for (int i = 0; i < CM_DIRECTION_COUNT; ++i)
	{
		float d = *danger[i];
		if (d > min_danger)
			*interest[i] = 0.0;
	}

}

static float angle_of(enum cm_direction direction)
{
	const float division = 0.7853981634; // 45 degrees
	return direction * division;
}

float cm_calculate(struct context_map *map)
{
	cm_slots *interest = &map->maps[CM_INTEREST];
	cm_slots *danger = &map->maps[CM_DANGER];

	// find lowest danger
	float min_danger = find_min(danger);

	// zero all slots with non-min danger in interest map
	mask_danger(interest, danger, min_danger);

	// find max of interest
	enum cm_direction best_direction = find_max(interest);

	// TODO calculate gradient around this point for smoothing
	return angle_of(best_direction);
}
