#pragma once

#include "EventSubscribtion.h"
#include "EventToken.h"
#include <functional>
#include <map>

template <typename... TArgs>
class Signal
{
public:
	using Observer = std::function<void((TArgs...))>;

	EventSubscribtion<Signal<TArgs...>, TArgs...> Subscribe(const Observer& observer)
	{
		m_observers[m_nextToken] = observer;

		return { this, m_nextToken++ };
	}

	void Unsubscribe(EventToken token)
	{
		if (m_observers.contains(token))
		{
			m_observers.erase(token);
		}
	}

	void operator()(TArgs... args)
	{
		for (auto& [_, observer] : m_observers)
		{
			if constexpr (sizeof...(args) == 0)
			{
				observer();
			}
			else
			{
				observer(args...);
			}
		}
	}

private:
	EventToken m_nextToken = 1;
	std::map<EventToken, Observer> m_observers;
};
