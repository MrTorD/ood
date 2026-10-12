#pragma once

#include "Bounds.h"
#include "DragPoint.h"
#include <vector>

class SelectionBox
{
public:
	SelectionBox(Bounds bounds)
		: m_bounds(bounds)
	{
		auto [x, y, w, h] = bounds;

		m_dragPoints.emplace_back(x, y, m_dragPointRadius,
			[](Bounds bounds, Point point) {
				return Bounds(
					bounds.x + point.x,
					bounds.y + point.y,
					bounds.w - point.x,
					bounds.h - point.y);
			});
		m_dragPoints.emplace_back(x + w, y, m_dragPointRadius,
			[](Bounds bounds, Point point) {
				return Bounds(
					bounds.x,
					bounds.y + point.y,
					bounds.w + point.x,
					bounds.h - point.y);
			});
		m_dragPoints.emplace_back(x, y + h, m_dragPointRadius,
			[](Bounds bounds, Point point) {
				return Bounds(
					bounds.x + point.x,
					bounds.y,
					bounds.w - point.x,
					bounds.h + point.y);
			});
		m_dragPoints.emplace_back(x + w, y + h, m_dragPointRadius,
			[](Bounds bounds, Point point) {
				return Bounds(
					bounds.x,
					bounds.y,
					bounds.w + point.x,
					bounds.h + point.y);
			});
	}

	Bounds GetBounds() const
	{
		return m_bounds;
	}

	const std::vector<DragPoint>& GetDragPoints() const
	{
		return m_dragPoints;
	}

private:
	Bounds m_bounds;
	float m_dragPointRadius = 10;
	std::vector<DragPoint> m_dragPoints;
};