#ifndef SIM_EVENT_H
#define SIM_EVENT_H

enum ev_type
{
	EVENT_ATTACKED = 1,
	EVENT_JUST_A_TEST,

	EVENT_COUNT
};

struct ev_event
{
	enum ev_type type;
	union
	{
		// TODO
	} payload;
};

#endif