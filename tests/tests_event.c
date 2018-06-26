#include "event/queue.h"
#include "event/event.h"
#include "tests.h"

void test_event()
{
	struct ev_queue q;
	ev_queue_init(&q);

	ASSERT(ev_queue_read(&q)->length == 0);
	ASSERT(ev_queue_write(&q)->length == 0);

	struct ev_event e1 = { .type = EVENT_ATTACKED };
	struct ev_event e2 = { .type = EVENT_JUST_A_TEST };
	ev_queue_push(&q, &e1);
	ev_queue_push(&q, &e2);

	// double buffering
	ASSERT(ev_queue_read(&q)->length == 0);
	ASSERT(ev_queue_write(&q)->length == 2);
	ev_queue_clear_n_flip(&q);
	ASSERT(ev_queue_read(&q)->length == 2);
	ASSERT(ev_queue_write(&q)->length == 0);

	struct ev_event check[2] = {0};
	ev_queue_process(&q, {check[i] = it;});
	ASSERT(check[0].type == EVENT_ATTACKED);
	ASSERT(check[1].type == EVENT_JUST_A_TEST);

	ev_queue_push(&q, &e2);
	ev_queue_clear_n_flip(&q);
	ASSERT(ev_queue_read(&q)->length == 1);
	ASSERT(ev_queue_write(&q)->length == 0);

	ev_queue_destroy(&q);
}

