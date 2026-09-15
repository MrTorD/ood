#pragma once

#include "IShapeGeometry.h"

class CircleGeometry : public IShapeGeometry
{
public:
	CircleGeometry(Point centre, double radius)
	{
		m_centre = centre;
		m_radius = radius;
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		auto [x, y] = m_centre;

		canvas.DrawEllipse({ x, y }, { m_radius, m_radius }, color);
	}

	std::string GetName() const override
	{
		return "circle";
	}

	void PrintParams(std::ostream& output) const override
	{
		auto [x, y] = m_centre;

		output << std::format("{} {} {} ", x, y, m_radius);
	}

	void Move(double dx, double dy) override
	{
		m_centre.x += dx;
		m_centre.y += dy;
	}

private:
	Point m_centre;
	double m_radius;
};
