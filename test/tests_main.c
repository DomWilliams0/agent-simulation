#include "helper.h"
#include "util/log.h"

#define MAX_TEST_COUNT (512)

int main()
{
	struct unit_test *first_entry = ({extern struct unit_test __start_tests; &__start_tests;});
	struct unit_test *last_entry = ({extern struct unit_test __stop_tests; &__stop_tests;});

	unsigned int test_count = last_entry - first_entry;
	if (test_count == 0)
		return 1;

	if (test_count >= MAX_TEST_COUNT)
	{
		LOG_INFO("Maximum number of tests reached, increase MAX_TEST_COUNT");
		return 2;
	}

	struct CMUnitTest tests[MAX_TEST_COUNT] = { 0 };

	int i = 0;
	for (struct unit_test *t = first_entry; t != last_entry; ++t)
	{
		const struct CMUnitTest test = {t->name, t->test, t->setup, t->teardown, NULL};
		tests[i++] = test;
	}

	return cmocka_run_group_tests(tests, setup_simulator, teardown_simulator);
}
