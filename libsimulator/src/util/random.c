#include <stdlib.h>
#include <time.h>

#include "util/util.h"

void random_init()
{
	srand(time(NULL));
}

float random_get(float max)
{
	return (float) rand() / (float) (RAND_MAX / max);
}
