#pragma once

#include "EventToken.h"
#include <functional>
#include <utility>

template <typename TSubject, typename... TArgs>
class EventSubscribtion
{
public:
	using Observer = std::function<void((TArgs...))>;

	EventSubscribtion(TSubject* subject, EventToken token)
	{
		m_subject = subject;
		m_token = token;
	}

	EventSubscribtion(EventSubscribtion&& subscribtion)
	{
		m_subject = std::exchange(subscribtion.m_subject, nullptr);
		m_token = std::exchange(subscribtion.m_token, 0);
	}

	void Cancel()
	{
		if (m_subject)
		{
			m_subject->Unsubscribe(m_token);
		}

		m_subject = nullptr;
	}

	EventSubscribtion& operator=(EventSubscribtion&& subscribtion)
	{
		if (&subscribtion == this)
		{
			return *this;
		}

		m_subject = std::exchange(subscribtion.m_subject, nullptr);
		m_token = std::exchange(subscribtion.m_token, 0);

		return *this;
	}

	~EventSubscribtion()
	{
		Cancel();
	}

private:
	TSubject* m_subject;
	EventToken m_token;
};
