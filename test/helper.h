#ifndef TESTS_HELPER_H
#define TESTS_HELPER_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

typedef void (*unit_test_t)(void **);
typedef int (*unit_test_env_t)(void **);

struct unit_test
{
	unit_test_t test;
	const char *name;

	unit_test_env_t setup;
	unit_test_env_t teardown;
};

#define REGISTER_TEST_SETUP_TEARDOWN(test_func, setup_func, teardown_func) \
	static struct unit_test unit_test_##test_func \
	__attribute((used, section("tests"))) = { \
		.test = test_func, \
		.name = #test_func, \
		.setup = setup_func, \
		.teardown = teardown_func \
	};

#define REGISTER_TEST_SETUP(test_func, setup) \
	REGISTER_TEST_SETUP_TEARDOWN(test_func, setup, NULL)

#define REGISTER_TEST_TEARDOWN(test_func, teardown) \
	REGISTER_TEST_SETUP_TEARDOWN(test_func, NULL, teardown)

#define REGISTER_TEST(test_func) \
	REGISTER_TEST_SETUP_TEARDOWN(test_func, NULL, NULL)


int setup_simulator(void **state);

int teardown_simulator(void **state);

int teardown_remove_all_entities(void **state);

#endif
