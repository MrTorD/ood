#pragma once

#include "Color.h"
#include "IShapeGeometry.h"
#include <memory>

class Shape
{
	// TODO: Избавиться от bounds, хранить положение фигуры в стратегии
	// Completed: Hello
public:
	Shape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Color color)
	{
		m_id = id;
		m_geometry = std::move(geometry);
		m_color = color;
	}

	void Draw(ICanvas& canvas) const
	{
		m_geometry->Draw(canvas, m_color);
	}

	void Move(double dx, double dy)
	{
		m_geometry->Move(dx, dy);
	}

	void Print(std::ostream& output)
	{
		output << std::format("{} {} {:#06x}", m_geometry->GetName(), m_id, m_color);
		m_geometry->PrintParams(output);
	}

	void SetColor(Color color)
	{
		m_color = color;
	}

	void SetGeometry(std::unique_ptr<IShapeGeometry> geometry)
	{
		m_geometry = std::move(geometry);
	}

private:
	std::string m_id;
	std::unique_ptr<IShapeGeometry> m_geometry;
	Color m_color;
};
