#include "queue.h"
#include "event.h"

void ev_queue_init(struct ev_queue *queue)
{
	for (int i = 0; i < 2; ++i)
	{
		vec_init(queue->_bufs + i);
		vec_reserve(queue->_bufs + i, EV_INITIAL_QUEUE_LENGTH);
	}
	queue->_double_buffered = 0;
}

void ev_queue_destroy(struct ev_queue *queue)
{
	for (int i = 0; i < 2; ++i)
		vec_deinit(queue->_bufs + i);
}

// evt is copied
void ev_queue_push(struct ev_queue *queue,
                   struct ev_event *evt)
{
	vec_push(ev_queue_write(queue), *evt);
}

void ev_queue_clear_n_flip(struct ev_queue *queue)
{
	vec_clear(ev_queue_read(queue));
	queue->_double_buffered = 1 - queue->_double_buffered;
}
