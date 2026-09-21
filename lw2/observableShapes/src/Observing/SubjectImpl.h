#pragma once

#include "Subscribtion.h"
#include <algorithm>
#include <list>

template <typename TSubject>
class SubjectImpl
{
public:
	Subscribtion<TSubject> Subscribe(IObserver<TSubject>* observer)
	{
		if (std::count(m_observers.begin(), m_observers.end(), observer) == 0) 
		{
			m_observers.push_back(observer);
		}

		return { static_cast<TSubject*>(this), observer };
	}

	void Unsubscribe(IObserver<TSubject>* observer)
	{
		m_observers.remove(observer);
		m_observersCopy.remove(observer);
	}

	void NotifyObservers()
	{
		m_observersCopy = m_observers;
		unsigned pos = 0;
		auto it = m_observersCopy.begin();
		auto end = m_observersCopy.end();

		while (it != end)
		{
			(*it)->Update();
			it = GetIterator(++pos);
		}
	}

protected:
	SubjectImpl() = default;

	~SubjectImpl()
	{
		for (auto observer : m_observers)
		{
			observer->CancelSubscribtion(static_cast<TSubject*>(this));
		}
	}

private:
	std::list<IObserver<TSubject>*>::iterator GetIterator(unsigned pos)
	{
		unsigned count = 0;

		for (auto it = m_observersCopy.begin(); it != m_observersCopy.end(); ++it)
		{
			if (count == pos)
			{
				return it;
			}

			++count;
		}

		return m_observersCopy.end();
	}

	std::list<IObserver<TSubject>*> m_observers;
	std::list<IObserver<TSubject>*> m_observersCopy;
};