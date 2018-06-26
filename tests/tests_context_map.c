#include <stdio.h>
#include "tests.h"
#include "entity/context_map.h"

void test_context_map()
{
	struct context_map map;
	cm_reset(&map);
	ASSERT(map.maps[CM_INTEREST][CM_S] == 0);

	float force;
	ASSERT(cm_calculate(&map, &force) == CM_DIRECTION_COUNT); // aka invalid

	cm_add(&map, CM_INTEREST, CM_E, 0.2);
	ASSERT(cm_calculate(&map, &force) == CM_E);
	printf("force %f\n", force);
	ASSERT_FEQ(force, 0.2);

	cm_add(&map, CM_INTEREST, CM_W, 0.4);
	ASSERT(cm_calculate(&map, &force) == CM_W);
	ASSERT_FEQ(force, 0.4);

	cm_add(&map, CM_DANGER, CM_W, 0.1);
	ASSERT(cm_calculate(&map, &force) == CM_E);
	ASSERT_FEQ(force, 0.2);
}