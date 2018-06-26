#ifndef CONTEXT_MAP_H
#define CONTEXT_MAP_H

enum cm_direction
{
	CM_N = 0,
	CM_NE,
	CM_E,
	CM_SE,
	CM_S,
	CM_SW,
	CM_W,
	CM_NW,

	CM_DIRECTION_COUNT
};

float cm_direction_angle(enum cm_direction direction);

enum cm_type
{
	CM_INTEREST = 0,
	CM_DANGER,

	CM_TYPE_COUNT
};

typedef float cm_slots[CM_DIRECTION_COUNT];

struct context_map
{
	// interest and danger
	cm_slots maps[CM_TYPE_COUNT];
};

// zeroes
void cm_reset(struct context_map *map);

void cm_add(struct context_map *map, enum cm_type type, enum cm_direction direction, float weight);

enum cm_direction cm_calculate(struct context_map *map);

#endif
