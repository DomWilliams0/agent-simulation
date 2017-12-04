#include <stdlib.h>
#include <time.h>

#include "util/util.h"

void random_init()
{
	srand(time(NULL));
}

double random_get(double max)
{
	return (double) rand() / (RAND_MAX / max);
}
