#pragma once

#include "../Rect.h"
#include "Canvas/ICanvas.h"

class IShapeGeometry
{
public:
	virtual void Draw(ICanvas& canvas, Rect bounds, Color color) = 0;
	virtual std::string GetName() const = 0;
	virtual void PrintParams(std::ostream& output, Rect bounds) const = 0;

	virtual ~IShapeGeometry() = default;
};
