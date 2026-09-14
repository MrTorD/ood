#pragma once

#include "IShapeGeometry.h"

class RectangleGeometry : public IShapeGeometry
{
public:
	void Draw(ICanvas& canvas, Rect bounds, Color color) override
	{
		auto [x, y] = bounds.GetTopLeft();
		double w = bounds.GetWidth();
		double h = bounds.GetHeight();

		Point topRight = { x + w, y };
		Point bottomLeft = { x, y + h };
		Point bottomRight = { x + w, y + h };

		canvas.DrawPolygon({ bounds.GetTopLeft(), topRight, bottomRight, bottomLeft }, color);
	}

	std::string GetName() const override
	{
		return "rectangle";
	}

	void PrintParams(std::ostream& output, Rect bounds) const override
	{
		output << bounds.GetTopLeft().x << "" << bounds.GetTopLeft().y << "" << bounds.GetWidth() << " " << bounds.GetHeight() << " ";
	}
};
