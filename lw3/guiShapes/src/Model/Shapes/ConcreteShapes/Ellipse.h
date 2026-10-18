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

	void SetBounds(Bounds bounds) override
	{
		auto [x, y, w, h] = bounds;

		m_radiusX = w / 2;
		m_radiusY = h / 2;

		m_centre.x = x + m_radiusX;
		m_centre.y = y + m_radiusY;
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

	void ShiftBy(Bounds offset) override
	{
		auto radiusXShift = (offset.w / 2);
		auto radiusYShift = (offset.h / 2);
		
		m_centre.x += offset.x + radiusXShift;
		m_centre.y += offset.y + radiusYShift;
		m_radiusX += radiusXShift;
		m_radiusY += radiusYShift;
	}

private:
	Point m_centre;
	float m_radiusX;
	float m_radiusY;
};