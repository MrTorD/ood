#pragma once

#include "Shape.h"
#include "Visitable.h"

class Ellipse : public Visitable<Ellipse, Shape>
{
public:
	Ellipse(Point centre, float radiusX, float radiusY, Color color)
	{
		m_centre = centre;
		m_radiusX = radiusX;
		m_radiusY = radiusY;
		m_color = color;
	}

	Bounds GetBounds() const override
	{
		float x = m_centre.x - m_radiusX;
		float y = m_centre.y - m_radiusY;

		float w = m_radiusX * 2;
		float h = m_radiusY * 2;

		return { x, y, w, h };
	}

	Point GetCentre() const
	{
		return m_centre;
	}

	float GetRadiusX() const
	{
		return m_radiusX;
	}

	float GetRadiusY() const
	{
		return m_radiusY;
	}

	bool HitTest(Point point) const override
	{
		float normalizedX = (point.x - m_centre.x) / m_radiusX;
		float normalizedY = (point.y - m_centre.y) / m_radiusY;

		return (normalizedX * normalizedX + normalizedY * normalizedY) <= 1.0f;
	}

	void MoveBy(Point offset) override
	{
		m_centre.x += offset.x;
		m_centre.y += offset.y;
	}

private:
	Point m_centre;
	float m_radiusX;
	float m_radiusY;
};