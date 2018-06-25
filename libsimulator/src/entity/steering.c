#include <chipmunk/chipmunk.h>
#include <math.h>
#include <entity/entity.h>

#include "entity/steering.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/log.h"
#include "util/memory.h"

#define CENTRE_TILE(coord) (coord + 0.5f)

struct separation_query
{
	world_body src_body;
	cpVect src_pos;
	cpVect mean;
	int count;
};

static void separation_callback(cpShape *shape, void *data)
{
	struct separation_query *query = data;
	world_body body = cpShapeGetBody(shape);
	if (body != query->src_body)
	{
		cpVect pos = cpBodyGetPosition(cpShapeGetBody(shape));
		query->count++;

		// mean += (me - neighbour).norm() / distance
		cpVect diff = cpvnormalize(cpvsub(query->src_pos, pos));
		double dist = cpvlength(diff);
		query->mean = cpvadd(query->mean, cpvmult(diff, 1/dist));
	}
}

static void handle_separation(world_body body, double *velocity)
{
	cpBB bb = cpBBNewForCircle(cpBodyGetPosition(body), HUMAN_RADIUS * 1.5f);

	struct separation_query query;
	query.src_body = body;
	query.src_pos = cpBodyGetPosition(body);
	query.mean = cpvzero;
	query.count = 0;

	cpSpaceBBQuery(cpBodyGetSpace(body), bb, CP_SHAPE_FILTER_ALL, &separation_callback, &query);

	if (query.count > 0)
	{
		double scale = HUMAN_ACCELERATION * 0.65; // experimental
		cpVect toAdd = cpvmult(query.mean, scale * (1 / query.count));
		velocity[0] += toAdd.x;
		velocity[1] += toAdd.y;
	}

	// TODO look into using vector fields
	// TODO context behaviours
}

void steering_update_system(struct entities *entities)
{
	struct component_physics *physics = (struct component_physics *)entity_get_component_array(entities, COMPONENT_PHYSICS);
	struct component_steer *steers = (struct component_steer *)entity_get_component_array(entities, COMPONENT_STEER);

	entity_id count = entities->count;
	const entity_mask render_mask = COMPONENT_PHYSICS | COMPONENT_STEER;

	double velocity[2];
	double position[2];
	for (entity_id i = 0; i < count; ++i)
	{
		if (!entity_has_component(entities, i, render_mask))
			continue;

		struct component_physics *phys = physics + i;
		world_get_position(phys->body, position);


		velocity[0] = velocity[1] = 0.f;
		struct component_steer *steer = steers + i;
		steering_apply(steer, position, velocity);

		if (steer->separation)
		{
			handle_separation(phys->body, velocity);
		}

		cpBodyApplyForceAtLocalPoint(phys->body, cpv(velocity[0], velocity[1]), cpvzero);
	}

}

static double length(double *v)
{
	return sqrt((v[0] * v[0]) + (v[1] * v[1]));
}

static void scale(double *velocity, double speed)
{
	double l = length(velocity);
	if (l < VELOCITY_MINIMUM)
		l = 1;

	velocity[0] *= speed / l;
	velocity[1] *= speed / l;
}

static inline void handle_seek(double pos[2], double goal_x, double goal_y, double *velocity)
{
	// full speed ahead
	velocity[0] = CENTRE_TILE(goal_x) - pos[0];
	velocity[1] = CENTRE_TILE(goal_y) - pos[1];
	scale(velocity, HUMAN_ACCELERATION);
}

// returns if arrived
static inline bool handle_arrive(double pos[2], double goal_x, double goal_y, double *velocity)
{
	velocity[0] = CENTRE_TILE(goal_x) - pos[0];
	velocity[1] = CENTRE_TILE(goal_y) - pos[1];

	double distance = length(velocity);
	double speed = HUMAN_ACCELERATION;
	bool arriving = distance < STEERING_ARRIVE_RADIUS;
	if (arriving)
		speed *= distance / STEERING_ARRIVE_RADIUS;

	scale(velocity, speed);

	return arriving;
}

static inline void handle_path_follow(double pos[2], struct component_steer *steer, double *velocity)
{
	struct steering_path_waypoint *current = steer->path_front;

	// no path
	if (!current)
	{
		return;
	}

	// move towards current waypoint
	if (handle_arrive(pos, current->pos[0], current->pos[1], velocity))
	{
		// arrived, move onto next
		steering_path_pop(steer);

		// try again
		handle_path_follow(pos, steer, velocity);
		return;
	}
}

void steering_apply(struct component_steer *steer, double current_pos[2], double *velocity)
{
	// how lovely
	switch(steer->type)
	{
		case STEERING_SEEK:
			handle_seek(current_pos, steer->goal_x, steer->goal_y, velocity);
			break;

		case STEERING_ARRIVE:
			handle_arrive(current_pos, steer->goal_x, steer->goal_y, velocity);
			break;

		case STEERING_PATH_FOLLOW:
			handle_path_follow(current_pos, steer, velocity);
			break;

		default:
			break;
	}
}

static struct steering_path_waypoint *create_node(double pos[2])
{
	struct steering_path_waypoint *waypoint;
	safe_malloc_struct(struct steering_path_waypoint, &waypoint);

	waypoint->next = NULL;
	waypoint->pos[0] = pos[0];
	waypoint->pos[1] = pos[1];

	return waypoint;
};

static void free_node(struct steering_path_waypoint *waypoint)
{
	safe_free(waypoint);
}

void steering_path_add(struct component_steer *steer, double waypoint[2])
{
	struct steering_path_waypoint *wp = create_node(waypoint);

	if (!steer->path_front)
	{
		steer->path_front = wp;
	}
	else if (steer->path_end)
	{
		steer->path_end->next = wp;
	}

	steer->path_end = wp;
}

bool steering_path_pop(struct component_steer *steer)
{
	struct steering_path_waypoint *front = steer->path_front;

	// empty
	if (!front)
		return false;

	steer->path_front = front->next;

	if (steer->path_front == NULL)
		steer->path_end = NULL;

	free_node(front);

	return true;
}


void steering_path_set(struct component_steer *steer, double waypoints[2], uint32_t n)
{
	// remove old
	while (steering_path_pop(steer));

	if (n == 0)
	{
		LOG_WARN("Path length must be at least 0");
		return;
	}

	// first element
	struct steering_path_waypoint *wp = create_node(waypoints);
	steer->path_front = wp;

	// the rest
	for (uint32_t i = 1; i < n; ++i)
	{
		struct steering_path_waypoint *next = create_node(waypoints + (i * 2));
		wp->next = next;
		wp = next;
	}
}
