#pragma once
/******************************************************************************/
/*!
\file  EventHandler.h
\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for EventHandler

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "Event.h"
#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <list>


#include "BaseHandler.h"
#include "DerivedHandler.h"


//Misc
#include <sol.hpp>

class EventHandler
{
public:
	EventHandler()
	{

	}

	~EventHandler()
	{

		for (auto & elem : subscribers)
		{
			//std::list of handlers
			if (elem.second)
			{
				for (auto itor = elem.second->begin(); itor != elem.second->end(); itor++)
				{
					delete *itor;
				}

				delete elem.second;
			}
		}

		for (auto & evnt : allEvents)
		{
			delete evnt;
		}
	}

	//Publish an event
	template<typename EventType>
	void PublishEvent(EventType * evnt)
	{
		//Returns the list of handlers tied to this event
		std::list<BaseHandler*> * handlers = subscribers[typeid(EventType)];

		if (handlers == nullptr) return; //If that handler does not exist, end

		for (auto & handler : *handlers)
		{
			handler->ExecuteEvent(evnt);
		}

		allEvents.push_back(evnt);
	}

	template<typename T, typename EventType>
	void SubscribeEvent(T* instance, void (T::*memberFunction)(EventType*))
	{
		std::list<BaseHandler*> * handlers = subscribers[typeid(EventType)];

		//If it is the first time initializing
		if (handlers == nullptr)
		{
			handlers = new std::list<BaseHandler*>; //Create a new list of handlers
			subscribers[typeid(EventType)] = handlers; //Register it to subscribers
		}

		handlers->push_back(new DerivedHandler<T, EventType>(instance, memberFunction));
	}

private:
	std::unordered_map <std::type_index, std::list<BaseHandler*>*> subscribers;
	std::vector<Event *> allEvents;
};
