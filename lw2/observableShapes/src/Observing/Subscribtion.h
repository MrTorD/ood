#pragma once

#include "IObserver.h"
#include <iostream>
#include <utility>

template <typename TSubject>
class Subscribtion final
{
public:
	Subscribtion(TSubject* subject, IObserver<TSubject>* observer)
	{
		m_subject = subject;
		m_observer = observer;
	}

	Subscribtion(Subscribtion&& subscribtion)
	{
		m_subject = std::exchange(subscribtion.m_subject, nullptr);
		m_observer = std::exchange(subscribtion.m_observer, nullptr);
	}

	TSubject* GetSubject() const
	{
		return m_subject;
	}

	void Cancel()
	{
		if (m_subject)
		{
			m_subject->Unsubscribe(m_observer);
			m_subject = nullptr;
		}
	}

	Subscribtion& operator=(Subscribtion&& subscribtion)
	{
		if (&subscribtion == this)
		{
			return *this;
		}

		m_subject = std::exchange(subscribtion.m_subject, nullptr);
		m_observer = std::exchange(subscribtion.m_observer, nullptr);

		return *this;
	}

	~Subscribtion()
	{
		Cancel();
	}

private:
	TSubject* m_subject;
	IObserver<TSubject>* m_observer;
};