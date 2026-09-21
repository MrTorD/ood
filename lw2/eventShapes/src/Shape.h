#pragma once

#include "Color.h"
#include "IShapeGeometry.h"
#include "Signal.h"
#include <list>
#include <memory>

class Shape
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

	using ColorEventSub = EventSubscribtion<Signal<const std::string&, Color>, const std::string&, Color>;
	ColorEventSub OnColorChanged(std::function<void(const std::string&, Color)> observer)
	{
		return m_onColorChanged.Subscribe(observer);
	}

	using MoveEventSub = EventSubscribtion<Signal<const std::string&, double, double>, const std::string&, double, double>;
	MoveEventSub OnShapeMoved(std::function<void(const std::string&, double, double)> observer)
	{
		return m_onMove.Subscribe(observer);
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
	Signal<const std::string&, Color> m_onColorChanged;
	Signal<const std::string&, double, double> m_onMove;

	std::string m_id;
	std::unique_ptr<IShapeGeometry> m_geometry;
	Color m_color;
};
