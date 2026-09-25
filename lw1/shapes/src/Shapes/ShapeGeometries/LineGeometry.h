#pragma once

#include "PrototypeImpl.h"

class LineGeometry : public PrototypeImpl<LineGeometry, IShapeGeometry>
{
public:
	LineGeometry(Bounds bounds)
	{
		SetBounds(bounds);
	}

	std::string GetName() const override
	{
		return "line";
	}

	Bounds GetBounds() const override
	{
		return { m_from, m_to.x - m_from.x, m_to.y - m_from.y };
	}

	void SetBounds(Bounds bounds) override
	{
		m_from = bounds.GetTopLeft();
		m_to = { m_from.x + bounds.GetWidth(), m_from.y + bounds.GetHeight() };
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.DrawLine(m_from, m_to, color);
	}

	void Move(double dx, double dy) override
	{
		m_from.x += dx;
		m_from.y += dy;
		m_to.x += dx;
		m_to.y += dy;
	}

	void PrintParams(std::ostream& output) const override
	{
		output << std::format("{} {} {} {}", m_from.x, m_from.y, m_to.x, m_to.y);
	}

private:
	Point m_from;
	Point m_to;
};
