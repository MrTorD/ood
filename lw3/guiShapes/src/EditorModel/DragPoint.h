#pragma once

#include "Point.h"
#include <functional>

struct DragPoint
{
	float x;
	float y;
	float radius;
	std::function<Bounds(Point)> GetOffset;

	// TODO: Rename HitTest?
	bool HitTest(Point point) const
	{
		return x - radius <= point.x
			&& point.x <= x + radius
			&& y - radius <= point.y
			&& point.y <= y + radius;
	}
};
