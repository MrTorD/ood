#pragma once

#include "Point.h"
#include "Errors/InvalidShapeError.h"

class Bounds
{
public:
	Bounds() = default;

	Bounds(Point topLeft, double width, double height)
	{
		m_topLeft = topLeft;
		SetWidth(width);
		SetHeight(height);
	}

	Point GetTopLeft() const
	{
		return m_topLeft;
	}

	Bounds& operator=(const Bounds& bounds)
	{
		SetTopLeft(bounds.m_topLeft);
		SetWidth(bounds.m_width);
		SetHeight(bounds.m_height);

		return *this;
	}

	void SetTopLeft(Point topLeft)
	{
		m_topLeft = topLeft;
	}

	double GetWidth() const
	{
		return m_width;
	}

	double GetHeight() const
	{
		return m_height;
	}

	void SetWidth(double width)
	{
		if (width < 0)
		{
			throw new InvalidShapeError("Width can't be number less than 0");
		}

		m_width = width;
	}

	void SetHeight(double height)
	{
		if (height < 0)
		{
			throw new InvalidShapeError("Height can't be number less than 0");
		}

		m_height = height;
	}

private:
	Point m_topLeft;
	double m_width;
	double m_height;
};
