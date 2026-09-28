#pragma once

#include "InvalidShapeError.h"
#include "Point.h"

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
			throw InvalidShapeError("Width can't be number less than 0");
		}

		m_width = width;
	}

	void SetHeight(double height)
	{
		if (height < 0)
		{
			throw InvalidShapeError("Height can't be number less than 0");
		}

		m_height = height;
	}

	bool operator==(const Bounds& other) const
	{
		return m_topLeft == other.m_topLeft && std::abs(m_width - other.m_width) < m_epsilon && std::abs(m_height - other.m_height) < m_epsilon;
	}

private:
	Point m_topLeft;
	double m_width;
	double m_height;

	static constexpr double m_epsilon = 1e-9;
};
