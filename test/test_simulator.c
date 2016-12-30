#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "simulator.h"

void test_creation_deletion(void **state)
{
}

void test_step(void **state)
{

}

int main()
{
	const UnitTest tests[] =
	{
		unit_test(test_creation_deletion),
		unit_test(test_step)
	};

	return run_tests(tests);
}

