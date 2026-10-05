#pragma once

#include "Shape.h"
#include "Visitable.h"
#include <algorithm>

class Triangle : public Visitable<Triangle, Shape>
{
public:
	Triangle(Point vertex1, Point vertex2, Point vertex3, Color color)
	{
		m_v1 = vertex1;
		m_v2 = vertex2;
		m_v3 = vertex3;
		m_color = color;
	}

	Bounds GetBounds() const override
	{
		auto [minX, maxX] = std::minmax({ m_v1.x, m_v2.x, m_v3.x });
		auto [minY, maxY] = std::minmax({ m_v1.y, m_v2.y, m_v3.y });

		return { minX, minY, maxX - minX, maxY - minY };
	}

	bool HitTest(Point p) const override
	{
		int a = (m_v1.x - p.x) * (m_v2.y - m_v1.y) - (m_v2.x - m_v1.x) * (m_v1.y - p.y);
		int b = (m_v2.x - p.x) * (m_v3.y - m_v2.y) - (m_v3.x - m_v2.x) * (m_v2.y - p.y);
		int c = (m_v3.x - p.x) * (m_v1.y - m_v3.y) - (m_v1.x - m_v3.x) * (m_v3.y - p.y);

		return (a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0);
	}

	void MoveBy(Point offset) override
	{
		m_v1.x += offset.x;
		m_v1.y += offset.y;

		m_v2.x += offset.x;
		m_v2.y += offset.y;

		m_v3.x += offset.x;
		m_v3.y += offset.y;
	}

	Point GetVertex1() const
	{
		return m_v1;
	}

	Point GetVertex2() const
	{
		return m_v2;
	}

	Point GetVertex3() const
	{
		return m_v3;
	}

private:
	Point m_v1;
	Point m_v2;
	Point m_v3;
};