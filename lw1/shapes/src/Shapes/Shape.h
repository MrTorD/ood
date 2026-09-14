#pragma once

#include "Color.h"
#include "Rect.h"
#include "ShapeGeometries/IShapeGeometry.h"
#include <memory>

class Shape
{
public:
	Shape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Rect bounds, Color color)
	{
		m_id = id;
		m_geometry = std::move(geometry);
		m_bounds = bounds;
		m_color = color;
	}

	void Draw(ICanvas& canvas) const
	{
		m_geometry->Draw(canvas, m_bounds, m_color);
	}

	void Move(double dx, double dy)
	{
		Point oldTopLeft = m_bounds.GetTopLeft();
		Point newTopLeft = { oldTopLeft.x + dx, oldTopLeft.y + dy };

		m_bounds.SetTopLeft(newTopLeft);
	}

	void Print(std::ostream& output)
	{
		output << std::format("{} {} {:#06x}", m_geometry->GetName(), m_id, m_color);
		m_geometry->PrintParams(output, m_bounds);
	}

	void SetColor(Color color)
	{
		m_color = color;
	}

	void SetGeometry(std::unique_ptr<IShapeGeometry> geometry)
	{
		m_geometry = std::move(geometry);
	}

	void SetBounds(Rect bounds)
	{
		m_bounds = bounds;
	}

private:
	std::string m_id;
	std::unique_ptr<IShapeGeometry> m_geometry;
	Rect m_bounds;
	Color m_color;
};
