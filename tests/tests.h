#ifndef TESTS_H
#define TESTS_H

#include <stdlib.h>

#define ASSERT(expr) do { if (!(expr)) fail(#expr, __FILE__, __LINE__); } while (0)
#define ASSERT_POS(a, b) ASSERT((a)[0] == (b)[0] && (a)[1] == (b)[1])
#define ASSERT_FEQ(a, b) ASSERT(abs((a) - (b)) < 0.00001)

void fail(const char *expr, const char *file, int line);

void test_entity();
void test_world();
void test_context_map();

#endif