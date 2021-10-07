#include "MoveEvent.h"

MoveEvent::MoveEvent(std::string id, int input, EventType type)
	:m_ID(id), m_value(input), m_EventType(type), Event{ type }
{
}

MoveEvent::~MoveEvent()
{
}
