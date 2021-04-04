#include "EventsHolder.h"

std::shared_ptr<EventsHolder> EventsHolder::getInstnce()
{
	static std::shared_ptr<EventsHolder> instance{ new EventsHolder };
	return instance;
}
