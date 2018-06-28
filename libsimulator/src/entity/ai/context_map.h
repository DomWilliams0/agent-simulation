#ifndef SIM_CONTEXT_MAP_H
#define SIM_CONTEXT_MAP_H

#define CM_DIRECTION_FIRST (CM_N)
#define CM_DIRECTION_LAST (CM_DIRECTION_COUNT)

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

// closest from given radians
// TODO tests
enum cm_direction cm_direction_from_angle(float angle);

enum cm_type
{
	CM_INTEREST = 0,
	CM_DANGER,

	CM_TYPE_COUNT
};

typedef float cm_slots[CM_DIRECTION_COUNT];

// TODO add cm_ prefix
struct context_map
{
	// interest and danger
	cm_slots maps[CM_TYPE_COUNT];
};

// zeroes
void cm_reset(struct context_map *map);

void cm_add(struct context_map *map, enum cm_type type, enum cm_direction direction, float weight);

enum cm_direction cm_calculate(struct context_map *map, float *force_out);

#endif
