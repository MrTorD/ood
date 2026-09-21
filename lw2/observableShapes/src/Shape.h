#pragma once

#include "Color.h"
#include "IObserver.h"
#include "IShapeGeometry.h"
#include "SubjectImpl.h"
#include <list>
#include <memory>

class Shape : public SubjectImpl<Shape>
{
public:
	Shape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Color color)
	{
		m_id = id;
		m_geometry = std::move(geometry);
		m_color = color;
	}

	const std::string& GetId() const
	{
		return m_id;
	}

	void SetColor(Color color)
	{
		m_color = color;
		NotifyObservers();
	}

	void SetGeometry(std::unique_ptr<IShapeGeometry> geometry)
	{
		m_geometry = std::move(geometry);
		NotifyObservers();
	}

	void Draw(ICanvas& canvas) const
	{
		m_geometry->Draw(canvas, m_color);
	}

	void Move(double dx, double dy)
	{
		m_geometry->Move(dx, dy);
		NotifyObservers();
	}

	void Print(std::ostream& output) const
	{
		output << std::format("{} {} {:#06x}", m_geometry->GetName(), m_id, m_color);
		m_geometry->PrintParams(output);
	}

private:
	std::string m_id;
	std::unique_ptr<IShapeGeometry> m_geometry;
	Color m_color;
};
