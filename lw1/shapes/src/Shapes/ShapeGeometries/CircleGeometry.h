#pragma once

#include "IShapeGeometry.h"

using CircleBounds = std::tuple<double, double, double>;

class CircleGeometry : public IShapeGeometry
{
public:
	void Draw(ICanvas& canvas, Rect bounds, Color color) override
	{
		auto [cx, cy, r] = CalcCircleBounds(bounds);

		canvas.DrawEllipse({ cx, cy }, { r, r }, color);
	}

	std::string GetName() const override
	{
		return "circle";
	}

	void PrintParams(std::ostream& output, Rect bounds) const override
	{
		auto [cx, cy, r] = CalcCircleBounds(bounds);

		output << std::format("{} {} {} ", cx, cy, r);
	}

private:
	CircleBounds CalcCircleBounds(Rect bounds) const
	{
		Point topLeft = bounds.GetTopLeft();
		double w = bounds.GetWidth();
		double h = bounds.GetHeight();

		double cx = topLeft.x + (w / 2);
		double cy = topLeft.y + (h / 2);
		double r = std::min(w / 2, h / 2);

		return { cx, cy, r };
	}
};
