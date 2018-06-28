#ifndef SIM_COMPONENTS_DECL_H
#define SIM_COMPONENTS_DECL_H

// names
#define ECS_COMP(name) struct ecs_comp_ ## name
#define ECS_COMP_INIT(name) ecs_comp_init_ ## name
#define ECS_COMP_DESTROY(name) ecs_comp_destroy_ ## name
#define ECS_COMP_MASK(name) ecs_mask_ ## name

// declarations and definitions
#define ECS_COMP_INIT_DECLARE(name) \
	void ECS_COMP_INIT(name)(ECS_COMP(name) *comp); \
	void ECS_COMP_DESTROY(name)(ECS_COMP(name) *comp); \

#define ECS_COMP_INIT_DEFINE(name, init, destroy) \
	void ECS_COMP_INIT(name)(ECS_COMP(name) *comp) { init } \
	void ECS_COMP_DESTROY(name)(ECS_COMP(name) *comp) { destroy }

#define ECS_COMP_DEFINE(uid, name, impl) \
	ECS_COMP(name) { impl }; \
	ECS_COMP_INIT_DECLARE(name); \
	static const int ECS_COMP_MASK(name) = 1 << (uid); \

#endif
