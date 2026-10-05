#pragma once

#include "EditorSession.h"
#include "Picture.h"

class CanvasController
{
public:
	CanvasController(Picture& picture, EditorSession& editorSession)
		: m_picture(picture)
		, m_editorSession(editorSession)
	{
	}

	void OnMousePressed(Point point)
	{
		auto selectedShape = m_picture.FindShapeAt(point);
		m_editorSession.SetSelectedShape(selectedShape);
		m_isDragging = selectedShape != nullptr;
		m_lastMousePosition = point;
	}

	void OnMouseMoved(Point point)
	{
		if (auto selectedShape = m_editorSession.GetSelectedShape();
			m_isDragging && selectedShape)
		{
			selectedShape->MoveBy(point - m_lastMousePosition);
			m_lastMousePosition = point;
		}
	}

	void OnMouseReleased(Point point)
	{
		m_isDragging = false;
	}

private:
	Picture& m_picture;
	EditorSession& m_editorSession;

	bool m_isDragging = false;
	Point m_lastMousePosition;
};