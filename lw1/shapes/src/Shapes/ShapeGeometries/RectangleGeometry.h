#pragma once

#include "IShapeGeometry.h"

class RectangleGeometry : public IShapeGeometry
{
public:
	RectangleGeometry(Bounds bounds)
	{
		SetBounds(bounds);
	}

	std::string GetName() const override
	{
		return "rectangle";
	}

	Bounds GetBounds() const override
	{
		return m_bounds;
	}

	void SetBounds(Bounds bounds)
	{
		m_bounds = bounds;
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		auto [x, y] = m_bounds.GetTopLeft();
		double w = m_bounds.GetWidth();
		double h = m_bounds.GetHeight();

		Point topRight = { x + w, y };
		Point bottomLeft = { x, y + h };
		Point bottomRight = { x + w, y + h };

		canvas.DrawPolygon({ m_bounds.GetTopLeft(), topRight, bottomRight, bottomLeft }, color);
	}

	void Move(double dx, double dy) override
	{
		auto [x, y] = m_bounds.GetTopLeft();
		Point newTopLeft = { x + dx, y + dy };

		m_bounds.SetTopLeft(newTopLeft);
	}

	void PrintParams(std::ostream& output) const override
	{
		auto [x, y] = m_bounds.GetTopLeft();

		output << std::format("{} {} {} {} ", x, y, m_bounds.GetWidth(), m_bounds.GetHeight());
	}

private:
	Bounds m_bounds;
};
