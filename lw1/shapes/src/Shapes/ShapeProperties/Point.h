#pragma once

#include <format>
#include <fstream>

struct Point
{
	double x;
	double y;

	bool operator==(const Point& other) const
	{
		return std::abs(x - other.x) < m_epsilon && std::abs(y - other.y) < m_epsilon;
	}

private:
	static constexpr double m_epsilon = 1e-9;
};
