#pragma once

#include "Bounds.h"
#include "ICanvas.h"

class IShapeGeometry
{
public:
	virtual std::string GetName() const = 0;

	virtual Bounds GetBounds() const = 0;
	virtual void SetBounds(Bounds bounds) = 0;

	virtual void Draw(ICanvas& canvas, Color color) = 0;
	virtual void Move(double dx, double dy) = 0;
	virtual void PrintParams(std::ostream& output) const = 0;

	virtual ~IShapeGeometry() = default;
};
