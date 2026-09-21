#pragma once

#include "IDanceBehavior.h"
#include "IFlyBehavior.h"
#include "IQuackBehavior.h"
#include <iostream>
#include <memory>

class Duck
{
public:
	// [x] Добавить SetBehavior. Подумать должен ли конструктор быть public или protected
	Duck(std::unique_ptr<IFlyBehavior> flyBehavior, std::unique_ptr<IQuackBehavior> quackBehavior, std::unique_ptr<IDanceBehavior> danceBehavior)
	{
		SetFlyBehavior(std::move(flyBehavior));
		SetQuackBehavior(std::move(quackBehavior));
		SetDanceBehavior(std::move(danceBehavior));
	}

	virtual std::string GetName() const = 0;

	void SetFlyBehavior(std::unique_ptr<IFlyBehavior> flyBehavior)
	{
		m_flyBehavior = std::move(flyBehavior);
	}

	void SetQuackBehavior(std::unique_ptr<IQuackBehavior> quackBehavior)
	{
		m_quackBehavior = std::move(quackBehavior);
	}

	void SetDanceBehavior(std::unique_ptr<IDanceBehavior> danceBehavior)
	{
		m_danceBehavior = std::move(danceBehavior);
	}

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

	// FIXME Подумать что с оператором
	// Duck& operator=(Duck&& duck)
	// {
	// 	m_flyBehavior = std::move(duck.m_flyBehavior);
	// 	m_quackBehavior = std::move(duck.m_quackBehavior);
	// 	m_danceBehavior = std::move(duck.m_danceBehavior);

	// 	return *this;
	// }

	virtual ~Duck() = default;

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior;
	std::unique_ptr<IQuackBehavior> m_quackBehavior;
	std::unique_ptr<IDanceBehavior> m_danceBehavior;
};