#pragma once

class IFlyBehavior
{
public:
	virtual void Fly() = 0;
	virtual bool IsFlyable() = 0;
	virtual unsigned GetFliesCount() = 0;
	virtual ~IFlyBehavior() = default;
};