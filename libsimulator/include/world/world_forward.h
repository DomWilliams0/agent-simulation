#ifndef WORLD_FORWARD_H
#define WORLD_FORWARD_H

// forward declarations of ode types, to avoid project-wide dependence on ode

struct dxWorld;
struct dxBody;

//typedef struct dxWorld *world_id; /* dWorldID; */
typedef struct dxBody *world_body;   /* dBodyID; */

typedef uint64_t coord;


#endif
