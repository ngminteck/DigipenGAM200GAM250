/******************************************************************************/
/*!
\file  DerivedHandler.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for BaseHandler

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Event.h"
#include "BaseHandler.h"
template<class T, class EventType>
class DerivedHandler : public BaseHandler
{
public:
	typedef void (T::*MemberFn)(EventType *);


	DerivedHandler(T * system, MemberFn memFn)
		:m_system{ system }, m_function{ memFn }
	{

	}


	void Call(Event * evnt) const override
	{
		(m_system->*m_function)(static_cast<EventType*>(evnt));
	}

private:
	T * m_system; //Pointer to the system
	MemberFn m_function; //Pointer to member function of m_system's
};