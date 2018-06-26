#ifndef TESTS_H
#define TESTS_H

#define ASSERT(expr) do { if (!(expr)) fail(#expr, __FILE__, __LINE__); } while (0)

void fail(const char *expr, const char *file, int line);

void test_entity();
void test_misc();
void test_sim();
void test_world();

#endif