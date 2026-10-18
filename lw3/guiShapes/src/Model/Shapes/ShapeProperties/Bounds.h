#pragma once

#include "Point.h"

struct Bounds
{
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;

	bool HitTest(Point point)
	{
		return x <= point.x
			&& point.x <= x + w
			&& y <= point.y
			&& point.y <= y + h;
	}

	auto operator<=>(const Bounds& b) const = default;

	Bounds& operator+=(const Bounds& bounds)
	{
		x += bounds.x;
		y += bounds.y;
		w += bounds.w;
		h += bounds.h;

		return *this;
	}

	Bounds operator-()
	{
		return { -x, -y, -w, -h };
	}
};