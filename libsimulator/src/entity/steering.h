#ifndef SIM_STEERING_H
#define SIM_STEERING_H

struct ecs;
struct ecs_comp_steer;

enum st_type
{
	ST_SEEK = 1,
	ST_FLEE,
	ST_ARRIVE,
};

void st_apply(struct ecs_comp_steer *steer, double current_pos[2], double *velocity);

// realise steering components -> physical movement
void st_system_tick(struct ecs *ecs);

#endif
