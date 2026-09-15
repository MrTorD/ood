#pragma once

#include "Canvas/ICanvas.h"

class IShapeGeometry
{
public:
	virtual void Draw(ICanvas& canvas, Color color) = 0;
	virtual std::string GetName() const = 0;
	virtual void PrintParams(std::ostream& output) const = 0;
	virtual void Move(double dx, double dy) = 0;

	virtual ~IShapeGeometry() = default;
};
