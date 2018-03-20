#ifndef __OBSERVER_ABSTRACT__
#define __OBSERVER_ABSTRACT__
#include <Events/Event.h>
#include <Object/Entity.h>

/* Observer pattern */
class Observer{
	public:
		virtual ~Observer() {}
		virtual void onNotify(const Entity* entity, Event event) = 0;
};
#endif
