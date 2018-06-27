#ifndef SIM_EVENT_QUEUE
#define SIM_EVENT_QUEUE

#include "util/vec.h"

#define EV_INITIAL_QUEUE_LENGTH (8)

typedef vec_t(struct ev_event) ev_vec_t;
struct ev_event; // fwd declare

struct ev_queue
{
	// double buffered, don't access directly
	ev_vec_t _bufs[2];
	int _double_buffered;
};

ev_vec_t *ev_queue_read(struct ev_queue *queue) { return queue->_bufs + queue->_double_buffered; }

ev_vec_t *ev_queue_write(struct ev_queue *queue) { return queue->_bufs + (1 - queue->_double_buffered); }


void ev_queue_init(struct ev_queue *queue);
void ev_queue_destroy(struct ev_queue *queue);

// evt is copied
void ev_queue_push(struct ev_queue *queue, struct ev_event *evt);

// run impl on each event in read queue
#define ev_queue_process(queue, impl) do {\
		int i; struct ev_event it; \
		vec_foreach(ev_queue_read((queue)), it, i) impl \
	} while(0)

// clears read queue and flips r/w
void ev_queue_clear_n_flip(struct ev_queue *queue);

#endif
