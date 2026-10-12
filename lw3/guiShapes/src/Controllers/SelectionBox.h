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

		// TODO: избавиться от магической 10
		m_dragPoints.emplace_back(x, y, 10,
			[](Bounds bounds, Point point) {
				return Bounds(
					bounds.x + point.x,
					bounds.y + point.y,
					bounds.w - point.x,
					bounds.h - point.y);
			});
		m_dragPoints.emplace_back(x + w, y, 10,
			[](Bounds bounds, Point point) {
				return Bounds(
					bounds.x,
					bounds.y + point.y,
					bounds.w + point.x,
					bounds.h - point.y);
			});
		m_dragPoints.emplace_back(x, y + h, 10,
			[](Bounds bounds, Point point) {
				return Bounds(
					bounds.x + point.x,
					bounds.y,
					bounds.w - point.x,
					bounds.h + point.y);
			});
		m_dragPoints.emplace_back(x + w, y + h, 10,
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
	std::vector<DragPoint> m_dragPoints;
};