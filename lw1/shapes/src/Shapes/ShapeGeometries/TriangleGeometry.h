#pragma once

#include "../Rect.h"
#include "IShapeGeometry.h"

using TriangleBounds = std::tuple<Point, Point, Point>;

class TriangleGeometry : public IShapeGeometry
{
public:
	void Draw(ICanvas& canvas, Rect bounds, Color color) override
	{
		auto [bottomLeft, topMiddle, bottomRight] = CalcTriangleBounds(bounds);

		canvas.DrawPolygon({ bottomLeft, topMiddle, bottomRight }, color);
	}

	std::string GetName() const override
	{
		return "triangle";
	}

	void PrintParams(std::ostream& output, Rect bounds) const override
	{
		auto [bottomLeft, topMiddle, bottomRight] = CalcTriangleBounds(bounds);

		output << std::format("{} {} {} {} {} {}", bottomLeft.x, bottomLeft.y, topMiddle.x, topMiddle.y, bottomRight.x, bottomRight.y);
	}

private:
	TriangleBounds CalcTriangleBounds(Rect bounds) const
	{
		auto [x, y] = bounds.GetTopLeft();
		double w = bounds.GetWidth();
		double h = bounds.GetHeight();

		Point bottomLeft = { x, y + h };
		Point topMiddle = { x + (w / 2), y };
		Point bottomRight = { x + w, y + h };

		return { bottomLeft, topMiddle, bottomRight };
	}
};
