#pragma once

#include "IShapeGeometry.h"

using LineBounds = std::tuple<Point, Point>;

class LineGeometry : public IShapeGeometry
{
public:
	void Draw(ICanvas& canvas, Rect bounds, Color color) override
	{
		auto [from, to] = CalcLineBounds(bounds);

		canvas.DrawLine(from, to, color);
	}

	std::string GetName() const override
	{
		return "line";
	}

	void PrintParams(std::ostream& output, Rect bounds) const override
	{
		auto [from, to] = CalcLineBounds(bounds);

		output << std::format("{} {} {} {}", from.x, from.y, to.x, to.y);
	}

	void Move(double dx, double dy) override
	{
		m_from.x += dx;
		m_from.y += dy;
		m_to.x += dx;
		m_to.y += dy;
	}

private:
	Point m_from;
	Point m_to;
};
