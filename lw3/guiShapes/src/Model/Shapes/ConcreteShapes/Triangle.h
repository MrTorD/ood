#pragma once

#include "Shape.h"
#include "Visitable.h"
#include <algorithm>

class Triangle : public Visitable<Triangle, Shape>
{
public:
	Triangle(Point topLeft, float width, float height, Color color)
	{
		m_topLeft = topLeft;
		m_width = width;
		m_height = height;
		m_color = color;
	}

	Bounds GetBounds() const override
	{
		auto [x, y] = m_topLeft;

		return { x, y, m_width, m_height };
	}

	void SetBounds(Bounds bounds) override
	{
		auto [x, y, w, h] = bounds;

		m_topLeft = { x, y };
		m_width = w;
		m_height = h;
	}

	bool HitTest(Point p) const override
	{
		Point m_v1 = GetVertex1();
		Point m_v2 = GetVertex2();
		Point m_v3 = GetVertex3();

		int a = (m_v1.x - p.x) * (m_v2.y - m_v1.y) - (m_v2.x - m_v1.x) * (m_v1.y - p.y);
		int b = (m_v2.x - p.x) * (m_v3.y - m_v2.y) - (m_v3.x - m_v2.x) * (m_v2.y - p.y);
		int c = (m_v3.x - p.x) * (m_v1.y - m_v3.y) - (m_v1.x - m_v3.x) * (m_v3.y - p.y);

		return (a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0);
	}

	void ShiftBy(Bounds offset) override
	{
		m_topLeft.x += offset.x;
		m_topLeft.y += offset.y;
		m_width += offset.w;
		m_height += offset.h;
	}

	Point GetVertex1() const
	{
		return { m_topLeft.x, m_topLeft.y + m_height };
	}

	Point GetVertex2() const
	{
		return { m_topLeft.x + (m_width) / 2, m_topLeft.y };
	}

	Point GetVertex3() const
	{
		return { m_topLeft.x + m_width, m_topLeft.y + m_height };
	}

private:
	Point m_topLeft;
	float m_width;
	float m_height;
};