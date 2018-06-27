#ifndef SIM_TESTS_H
#define SIM_TESTS_H

#include <stdlib.h>

#define ASSERT(expr) do { if (!(expr)) fail(#expr, __FILE__, __LINE__); } while (0)
#define ASSERT_FEQ(a, b) ASSERT(abs((a) - (b)) < 0.00001)
#define ASSERT_POS(a, b) ASSERT_FEQ((a).x, (b).x); ASSERT_FEQ((a).y, (b).y);

void fail(const char *expr, const char *file, int line);

void test_entity();
void test_world();
void test_context_map();
void test_event();
void test_action();

#endif