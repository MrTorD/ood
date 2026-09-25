#pragma once

#include "Color.h"
#include "IShapeGeometry.h"
#include <boost/signals2.hpp>
#include <list>
#include <memory>

using boost::signals2::scoped_connection;

class BoostShape
{
public:
	BoostShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Color color)
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
		m_onColorChanged(m_id, color);
	}

	void SetGeometry(std::unique_ptr<IShapeGeometry> geometry)
	{
		m_geometry = std::move(geometry);
	}

	void Draw(ICanvas& canvas) const
	{
		m_geometry->Draw(canvas, m_color);
	}

	scoped_connection OnColorChanged(std::function<void(const std::string&, Color)> observer)
	{
		return m_onColorChanged.connect(observer);
	}

	scoped_connection OnShapeMoved(std::function<void(const std::string&, double, double)> observer)
	{
		return m_onMove.connect(observer);
	}

	void Move(double dx, double dy)
	{
		m_geometry->Move(dx, dy);
		m_onMove(m_id, dx, dy);
	}

	void Print(std::ostream& output) const
	{
		output << std::format("{} {} {:#06x}", m_geometry->GetName(), m_id, m_color);
		m_geometry->PrintParams(output);
	}

private:
	boost::signals2::signal<void(const std::string&, Color)> m_onColorChanged;
	boost::signals2::signal<void(const std::string&, double, double)> m_onMove;

	std::string m_id;
	std::unique_ptr<IShapeGeometry> m_geometry;
	Color m_color;
};
