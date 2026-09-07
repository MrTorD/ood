#pragma once

class IDanceBehavior
{
public:
	virtual void Dance() = 0;
	virtual ~IDanceBehavior() = default;
};