#pragma once

struct Point
{
	float x = 0;
	float y = 0;

	auto operator<=>(const Point& p) const = default;
};

Point operator+(Point left, Point right)
{
	return { left.x + right.x, left.y + right.y };
}

Point operator-(Point left, Point right)
{
	return { left.x - right.x, left.y - right.y };
}