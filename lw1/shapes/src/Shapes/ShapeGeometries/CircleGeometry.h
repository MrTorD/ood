#pragma once

#include "IShapeGeometry.h"

class CircleGeometry : public IShapeGeometry
{
public:
	CircleGeometry(Bounds bounds)
	{
		SetBounds(bounds);
	}

	std::string GetName() const override
	{
		return "circle";
	}

	Bounds GetBounds() const override
	{
		Point topLeft = { m_centre.x - m_radius, m_centre.y + m_radius };

		return { topLeft, m_radius * 2, m_radius * 2 };
	}

	void SetBounds(Bounds bounds) override
	{
		auto [x, y] = bounds.GetTopLeft();
		double w = bounds.GetWidth();
		double h = bounds.GetHeight();

		m_centre = { x + (w / 2.), y - (h / 2.) };
		m_radius = std::min(w / 2., h / 2.);
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		auto [x, y] = m_centre;

		canvas.DrawEllipse({ x, y }, { m_radius, m_radius }, color);
	}

	void Move(double dx, double dy) override
	{
		m_centre.x += dx;
		m_centre.y += dy;
	}

	void PrintParams(std::ostream& output) const override
	{
		auto [x, y] = m_centre;

		output << std::format("{} {} {} ", x, y, m_radius);
	}

private:
	Point m_centre;
	double m_radius;
};
