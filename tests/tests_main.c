#include <stdlib.h>
#include <stdio.h>
#include "tests.h"

int fails = 0;

int main()
{
	test_entity();
	test_world();
	test_context_map();

	if (fails == 0)
	{
		puts("Tests pass");
		return 0;
	}
	else
	{
		printf("Tests fail: %d failures\n", fails);
		return 1;
	}
}

void fail(const char *expr, const char *file, int line)
{
	fprintf(stderr, "FAIL %s:%d | %s\n", file, line, expr);
	fails++;
}
