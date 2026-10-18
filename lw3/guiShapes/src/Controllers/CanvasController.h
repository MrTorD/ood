#pragma once

#include "PictureRenderer.h"
#include "SelectionBox.h"
#include <algorithm>

class CanvasController
{
public:
	CanvasController(EditorModel& model, Bounds canvasBounds)
		: m_model(model)
		, m_bounds(canvasBounds)
	{
	}

	void OnMouseWithCtrlPressed(Point point)
	{
		auto* shape = m_model.FindShapeAt({ point.x - m_bounds.x, point.y - m_bounds.y });

		if (shape)
		{
			shape->SetSelected(!shape->IsSelected());
		}
	}

	void OnMousePressed(Point point)
	{
		Point normalizedPoint = { point.x - m_bounds.x, point.y - m_bounds.y };
		auto* shape = m_model.FindShapeAt(normalizedPoint);

		if (shape)
		{
			if (!shape->IsSelected())
			{
				m_model.UnselectAll();
				shape->SetSelected(true);
			}

			m_model.BeginShiftShapes(normalizedPoint);

			m_isShiftingShapes = true;
			m_shiftStartPosition = point;
		}
		else
		{
			m_model.UnselectAll();
			m_isShiftingShapes = false;
		}
	}

	void OnMouseMoved(Point point)
	{
		if (!m_isShiftingShapes)
		{
			return;
		}

		m_model.UpdateShiftShapes(point - m_shiftStartPosition);
	}

	void OnMouseReleased(Point point)
	{
		if (!m_isShiftingShapes)
		{
			return;
		}

		m_model.UpdateShiftShapes(point - m_shiftStartPosition);

		m_model.CommitShiftShapes();

		m_isShiftingShapes = false;
	}

private:
	EditorModel& m_model;
	Bounds m_bounds;

	bool m_isShiftingShapes = false;
	Point m_shiftStartPosition;
};