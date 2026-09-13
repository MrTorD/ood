#pragma once

#include "./DanceBehaviors/IDanceBehavior.h"
#include "./FlyBehaviors/IFlyBehavior.h"
#include "./QuackBehaviors/IQuackBehavior.h"
#include <iostream>
#include <memory>

class Duck
{
public:

//TODO:
// SetBehavior. Нужно ли протектед ктор вместо протектед
	Duck() = default;

	virtual std::string GetName() const = 0;

	void Quack() const
	{
		m_quackBehavior->Quack();
	}

	void Fly()
	{
		m_flyBehavior->Fly();

		unsigned fliesCount = m_flyBehavior->GetFliesCount();

		if (m_flyBehavior->IsFlyable() && fliesCount % 2 == 0)
		{
			m_quackBehavior->Quack();
		}
	}

	void Swim()
	{
		std::cout << "I'm swimming now as any duck can\n";
	}

	void Dance()
	{
		m_danceBehavior->Dance();
	}

	Duck& operator=(Duck&& duck)
	{
		m_flyBehavior = std::move(duck.m_flyBehavior);
		m_quackBehavior = std::move(duck.m_quackBehavior);
		m_danceBehavior = std::move(duck.m_danceBehavior);

		return *this;
	}

	virtual ~Duck() = default;

protected:
	Duck(std::unique_ptr<IFlyBehavior> flyBehavior, std::unique_ptr<IQuackBehavior> quackBehavior, std::unique_ptr<IDanceBehavior> danceBehavior)
	{
		m_flyBehavior = std::move(flyBehavior);
		m_quackBehavior = std::move(quackBehavior);
		m_danceBehavior = std::move(danceBehavior);
	}

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior;
	std::unique_ptr<IQuackBehavior> m_quackBehavior;
	std::unique_ptr<IDanceBehavior> m_danceBehavior;
};