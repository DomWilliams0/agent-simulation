#include <chipmunk/chipmunk.h>
#include <entity/ecs.h>
#include "world/world.h"

#include "util/log.h"

#define STEERING_ARRIVE_RADIUS (0.5f)
#define CENTRE_TILE(coord) (coord + 0.5f)

static void st_separate(world_body body, cpVect *out);

void st_system_tick(struct ecs *ecs)
{
	struct ecs_comp_physics *physics = ecs_all(ecs, ECS_COMP_PHYSICS);
	struct ecs_comp_steer *steers = ecs_all(ecs, ECS_COMP_STEER);

	const ecs_mask mask = ECS_COMP_PHYSICS | ECS_COMP_STEER;

	for (ecs_id i = 0; i < ecs->count; ++i)
	{
		if (!ecs_has(ecs, i, mask))
			continue;

		struct ecs_comp_physics *p = &physics[i];
		struct ecs_comp_steer *s = &steers[i];

		cpVect position = world_get_position(p->body);
		cpVect velocity = cpvzero;

		st_apply(s, position, &velocity);

		if (s->separation)
			st_separate(p->body, &velocity);

		cpBodyApplyForceAtLocalPoint(p->body, velocity, cpvzero);
	}

}

static void scale(cpVect *velocity, double speed)
{
	double l = cpvlength(*velocity);
	if (l < VELOCITY_MINIMUM)
		l = 1;

	*velocity = cpvmult(*velocity, speed / l);
}

static void st_seek(cpVect pos, cpVect target, cpVect *out)
{
	*out = cpvsub(target, pos);
	scale(out, HUMAN_ACCELERATION); // full speed ahead
}

static void st_flee(cpVect pos, cpVect target, cpVect *out)
{
	cpVect tmp = cpvzero;
	st_seek(pos, target, &tmp);
	*out = cpvmult(tmp, -1);
}

// returns if arrived
static bool st_arrive(cpVect pos, cpVect target, cpVect *out)
{
	*out = cpvsub(target, pos);

	double distance = cpvlength(*out);
	double speed = HUMAN_ACCELERATION;
	bool arriving = distance < STEERING_ARRIVE_RADIUS;
	if (arriving)
		speed *= distance / STEERING_ARRIVE_RADIUS;

	scale(out, speed);
	return arriving;
}

void st_apply(struct ecs_comp_steer *steer, cpVect current_pos, cpVect *velocity_out)
{
	switch(steer->type)
	{
		case ST_SEEK:
			st_seek(current_pos, steer->target, velocity_out);
			return;

		case ST_FLEE:
			st_flee(current_pos, steer->target, velocity_out);
			return;

		case ST_ARRIVE:
			steer->arrived = st_arrive(current_pos, steer->target, velocity_out);
			return;

		case ST_NONE:
			return;
	}

	LOG_ERROR("Steering %d unimplemented", steer->type);
	LOG_FLUSH;
	exit(6);
}

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

static void st_separate(world_body body, cpVect *out)
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
		*out = cpvadd(*out, toAdd);
	}

	// TODO look into using vector fields
	// TODO context behaviours
}
