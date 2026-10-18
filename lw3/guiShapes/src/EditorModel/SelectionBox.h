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
			[](Point point) {
				return Bounds(
					+point.x,
					+point.y,
					-point.x,
					-point.y);
			});
		m_dragPoints.emplace_back(x + w, y, m_dragPointRadius,
			[](Point point) {
				return Bounds(
					0,
					+point.y,
					+point.x,
					-point.y);
			});
		m_dragPoints.emplace_back(x, y + h, m_dragPointRadius,
			[](Point point) {
				return Bounds(
					+point.x,
					0,
					-point.x,
					+point.y);
			});
		m_dragPoints.emplace_back(x + w, y + h, m_dragPointRadius,
			[](Point point) {
				return Bounds(
					0,
					0,
					+point.x,
					+point.y);
			});
		m_dragPoints.emplace_back(x + (w / 2), y, m_dragPointRadius,
			[](Point point) {
				return Bounds(
					0,
					+point.y,
					0,
					-point.y);
			});
		m_dragPoints.emplace_back(x + (w / 2), y + h, m_dragPointRadius,
			[](Point point) {
				return Bounds(
					0,
					0,
					0,
					+point.y);
			});
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

	const std::vector<DragPoint>& GetDragPoints() const
	{
		return m_dragPoints;
	}

private:
	Bounds m_bounds;
	float m_dragPointRadius = 10;
	std::vector<DragPoint> m_dragPoints;
};