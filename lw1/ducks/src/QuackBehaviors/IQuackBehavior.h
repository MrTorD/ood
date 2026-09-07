#pragma once

class IQuackBehavior
{
public:
	virtual void Quack() = 0;
	virtual ~IQuackBehavior() = default;
};