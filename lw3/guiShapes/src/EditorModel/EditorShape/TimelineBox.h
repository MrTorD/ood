#pragma once

#include "Bounds.h"
#include "DragPoint.h"
#include <vector>

class TimelineBox
{
public:
	explicit TimelineBox(size_t index, Point timeline)
		: m_bounds(timeline.x, CalcY(index), timeline.y, m_height)
	{
		auto [x, y, w, h] = m_bounds;

		m_dragPoints.emplace_back(x, y + (h / 2), m_dragPointRadius,
			[](Point point) {
				return Bounds(
					+point.x,
					0,
					-point.x,
					0);
			});
		m_dragPoints.emplace_back(x + w, y + (h / 2), m_dragPointRadius,
			[](Point point) {
				return Bounds(
					0,
					0,
					+point.x,
					0);
			});
	}

	Bounds GetBounds() const
	{
		return m_bounds;
	}

	static float GetHeight()
	{
		return m_height;
	}

	static float GetTotalHeight()
	{
		return m_height + m_padding;
	}

	bool IsContains(Point point) const
	{
		return m_bounds.IsContains(point);
	}

	const std::vector<DragPoint>& GetDragPoints() const
	{
		return m_dragPoints;
	}

private:
	Bounds m_bounds;
	static constexpr float m_height = 40;
	static constexpr float m_padding = 15;
	float m_dragPointRadius = 10;
	std::vector<DragPoint> m_dragPoints;

	float CalcY(size_t index)
	{
		return index * (m_height + m_padding);
	}
};