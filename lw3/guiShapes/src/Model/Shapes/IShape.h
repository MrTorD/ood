#pragma once

#include "Bounds.h"
#include "Color.h"
#include "IShapeVisitor.h"
#include <memory>

class IShape
{
public:
	virtual Bounds GetBounds() const = 0;
	virtual void ShiftBy(Bounds offset) = 0;

	virtual Color GetColor() const = 0;
	virtual Point GetTimeline() const = 0;
	virtual void SetTimeline(Point timeline) = 0;

	virtual bool IsContains(Point point) const = 0;
	virtual void Accept(IShapeVisitor& visitor) const = 0;
	virtual std::unique_ptr<IShape> Clone() const = 0;

	virtual ~IShape() = default;
};