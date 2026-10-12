#pragma once

#include "EditorSession.h"
#include "Picture.h"
#include "SelectionBox.h"
#include <algorithm>

class CanvasController
{
public:
	CanvasController(Picture& picture, EditorSession& editorSession, Bounds canvasBounds)
		: m_picture(picture)
		, m_editorSession(editorSession)
		, m_canvasBounds(canvasBounds)
	{
	}

	void OnMousePressed(Point point)
	{
		if (auto selectedShape = m_editorSession.GetSelectedShape(); selectedShape && selectedShape->GetBounds().HitTest(point))
		{
			auto selectionBox = m_editorSession.GetSelectionBox();
			auto dragPoints = selectionBox.value().GetDragPoints();
			auto it = std::find_if(
				dragPoints.cbegin(),
				dragPoints.cend(),
				[point](const DragPoint& dragPoint) {
					return dragPoint.HitTest(point);
				});

			if (it != dragPoints.end())
			{
				m_isResizing = true;
				m_currDragPoint = *it;
			}
			else
			{
				m_isDragging = true;
			}

			m_lastMousePosition = point;

			return;
		}

		auto selectedShape = m_picture.FindShapeAt(point);
		m_editorSession.SetSelectedShape(selectedShape);
		m_isDragging = selectedShape != nullptr;
		m_lastMousePosition = point;
	}

	void OnMouseMoved(Point point)
	{
		auto* selectedShape = m_editorSession.GetSelectedShape();

		auto [x, y, w, h] = m_canvasBounds;

		float maxX = x + w;
		float maxY = y + h;

		point.x = std::clamp(point.x, x, maxX);
		point.y = std::clamp(point.y, y, maxY);

		if (m_isResizing && m_currDragPoint && selectedShape)
		{
			auto newBounds = m_currDragPoint->GetNewBounds(selectedShape->GetBounds(), point - m_lastMousePosition);

			selectedShape->SetBounds(newBounds);
		}
		else if (m_isDragging && selectedShape)
		{
			selectedShape->MoveBy(point - m_lastMousePosition);
		}

		m_lastMousePosition = point;
	}

	void OnMouseReleased(Point point)
	{
		m_isDragging = false;
		m_isResizing = false;
		m_currDragPoint = std::nullopt;
	}

	Picture& GetPicture()
	{
		return m_picture;
	}

private:
	Picture& m_picture;
	EditorSession& m_editorSession;

	bool m_isDragging = false;
	bool m_isResizing = false;
	std::optional<DragPoint> m_currDragPoint;
	Point m_lastMousePosition;
	Bounds m_canvasBounds;
};