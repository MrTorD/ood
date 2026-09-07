#pragma once

#include <functional>
#include <iostream>

class Duck
{
public:
	Duck() = delete;

	virtual std::string GetName() const = 0;

	void Quack() const
	{
		m_quackBehavior();
	}

	void Fly()
	{
		m_flyBehavior();

		unsigned fliesCount = m_flyBehavior();

		if (fliesCount > 0 && fliesCount % 2 == 0)
		{
			m_quackBehavior();
		}
	}

	void Swim()
	{
		std::cout << "I'm swimming now as any duck can\n";
	}

	void Dance()
	{
		m_danceBehavior();
	}

	virtual ~Duck() = default;

protected:
	Duck(const std::function<int()>& flyBehavior, const std::function<void()>& quackBehavior, const std::function<void()>& danceBehavior)
	{
		m_flyBehavior = flyBehavior;
		m_quackBehavior = quackBehavior;
		m_danceBehavior = danceBehavior;
	}

private:
	std::function<int()> m_flyBehavior;
	std::function<void()> m_quackBehavior;
	std::function<void()> m_danceBehavior;
};