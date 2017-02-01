#include "helper.h"
#include "util/util.h"

UNIT_TEST(misc_random)
{
	random_init();

	for (int max = 0; max < 10; ++max)
	{
		for (int i = 0; i < 100; ++i)
		{
			assert_in_range(random_get(max), 0, max);
		}
	}
}

REGISTER_TEST(misc_random);
