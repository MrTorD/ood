#pragma once

#include "Bounds.h"
#include "PrototypeImpl.h"

class TriangleGeometry : public PrototypeImpl<TriangleGeometry, IShapeGeometry>
{
public:
	TriangleGeometry(Bounds bounds)
	{
		SetBounds(bounds);
	}

	std::string GetName() const override
	{
		return "triangle";
	}

	Bounds GetBounds() const override
	{
		Point topLeft = { m_bottomLeft.x, m_topMiddle.y };
		double width = m_bottomRight.x - m_bottomLeft.x;
		double height = m_topMiddle.y - m_bottomLeft.y;

		return { topLeft, width, height };
	}

	void SetBounds(Bounds bounds) override
	{
		auto [x, y] = bounds.GetTopLeft();
		double w = bounds.GetWidth();
		double h = bounds.GetHeight();

		m_bottomLeft = { x, y + h };
		m_topMiddle = { x + (w / 2), y };
		m_bottomRight = { x + w, y + h };
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.DrawPolygon({ m_bottomLeft, m_bottomLeft, m_bottomLeft }, color);
	}

	void Move(double dx, double dy) override
	{
		m_bottomLeft = { m_bottomLeft.x + dx, m_bottomLeft.y + dy };
		m_topMiddle = { m_topMiddle.x + dx, m_topMiddle.y + dy };
		m_bottomLeft = { m_bottomRight.x + dx, m_bottomRight.y + dy };
	}

	void PrintParams(std::ostream& output) const override
	{
		output << std::format("{} {} {} {} {} {}", m_bottomLeft.x, m_bottomLeft.y, m_topMiddle.x, m_topMiddle.y, m_bottomRight.x, m_bottomRight.y);
	}

private:
	Point m_bottomLeft;
	Point m_topMiddle;
	Point m_bottomRight;
};
