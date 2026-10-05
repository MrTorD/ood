#pragma once

#include "Shape.h"
#include "Visitable.h"

class Rectangle : public Visitable<Rectangle, Shape>
{
public:
	Rectangle(Point topLeft, double width, double height, Color color)
	{
		SetTopLeft(topLeft);
		SetWidth(width);
		SetHeight(height);
		SetColor(color);
	}

	Bounds GetBounds() const override
	{
		auto [x, y] = m_topLeft;

		return { x, y, m_width, m_height };
	}

	bool HitTest(Point point) const override
	{
		return GetBounds().HitTest(point);
	}

	void MoveBy(Point offset) override
	{
		m_topLeft.x += offset.x;
		m_topLeft.y += offset.y;
	}

	Point GetTopLeft() const
	{
		return m_topLeft;
	}

	float GetWidth() const
	{
		return m_width;
	}

	float GetHeight() const
	{
		return m_height;
	}

	void SetTopLeft(Point topLeft)
	{
		m_topLeft = topLeft;
	}

	void SetWidth(float width)
	{
		if (width <= 0)
		{
			throw std::invalid_argument("Width can't be negative number");
		}

		m_width = width;
	}

	void SetHeight(float height)
	{
		if (height <= 0)
		{
			throw std::invalid_argument("Height can't be negative number");
		}

		m_height = height;
	}

private:
	Point m_topLeft;
	float m_width;
	float m_height;
};