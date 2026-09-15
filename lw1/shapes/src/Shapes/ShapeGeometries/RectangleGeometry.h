#pragma once

#include "IShapeGeometry.h"

class RectangleGeometry : public IShapeGeometry
{
public:
	void Draw(ICanvas& canvas, Color color) override
	{
		auto [x, y] = m_topLeft();

		Point topRight = { x + m_width, y };
		Point bottomLeft = { x, y + m_height };
		Point bottomRight = { x + m_width, y + m_height };

		canvas.DrawPolygon({ m_topLeft, topRight, bottomRight, bottomLeft }, color);
	}

	std::string GetName() const override
	{
		return "rectangle";
	}

	void PrintParams(std::ostream& output) const override
	{
		auto [x, y] = m_topLeft;

		output << std::format("{} {} {} {} ", x, y, m_width, m_height);
	}

private:
	Point m_topLeft;
	double width;
	double height;
};
