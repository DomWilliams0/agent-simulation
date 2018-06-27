#ifndef SIM_WORLD_FORWARD_H
#define SIM_WORLD_FORWARD_H

// forward declarations of physics types, to avoid project-wide dependence on physics library

struct cpBody;

typedef struct cpBody *world_body;

typedef int coord;

#endif
