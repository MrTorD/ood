#pragma once

#include "IShapeVisitor.h"

template <typename TConcreteElement, typename TBase>
class Visitable : public TBase
{
public:
	void Accept(IShapeVisitor& visitor) const override
	{
		visitor.Visit(static_cast<const TConcreteElement&>(*this));
	}
};