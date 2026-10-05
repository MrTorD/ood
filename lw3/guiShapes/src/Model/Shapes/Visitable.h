#pragma once

#include "IShapeOperation.h"

template <typename TConcreteElement, typename TBase>
class Visitable : public TBase
{
	virtual void ApplyOperation(IShapeOperation& operation) const override
	{
		operation.ApplyTo(static_cast<const TConcreteElement&>(*this));
	}
};