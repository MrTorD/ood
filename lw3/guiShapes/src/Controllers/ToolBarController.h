#pragma once

#include "Button.h"
#include "EditorSession.h"
#include "Picture.h"
#include "PictureSerialiser.h"
#include "Rectangle.h"
#include <vector>

class ToolBarController
{
public:
	ToolBarController(Picture& picture, EditorSession& session)
		: m_picture(picture)
		, m_session(session)
	{
		// [x]: Контроллер не должен знать про расположение кнопок
	}

	void AddRectangle()
	{
		m_picture.AddShape(std::make_unique<Rectangle>(Point(400, 390), 200, 150, 0xB1FF21FF));
	}

	void AddEllipse()
	{
		m_picture.AddShape(std::make_unique<Ellipse>(Point(320, 220), 60, 30, 0xFFDE21FF));
	}

	void AddTriangle()
	{
		m_picture.AddShape(std::make_unique<Triangle>(Point(300, 250), 60, 60, 0xF9AAC0FF));
	}

	void DeleteSelectedShape()
	{
		auto* selected = m_session.GetSelectedShape();
		m_picture.DeleteShape(selected);
		m_session.SetSelectedShape(nullptr);
	}

	void SaveToDocument()
	{
	}

	void LoadFromDocument()
	{
	}

private:
	Picture& m_picture;
	EditorSession& m_session;
};