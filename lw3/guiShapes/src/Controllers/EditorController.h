#pragma once

#include "EditorWindow.h"

#include "Picture.h"

class EditorController
{
// [ ]: Подумать о целесообразности EditorController. Почитать об Dependency Injection
public:
	EditorController()
		: m_canvasController(m_picture, m_session, { 200, 100, 400, 400 })
		, m_toolBarController(m_picture, m_session)
		, m_view(m_canvasController, m_toolBarController, m_session)
	{
		m_view.Run();
	}

private:
	Picture m_picture;

	EditorSession m_session;
	CanvasController m_canvasController;
	ToolBarController m_toolBarController;
	EditorWindow m_view;
};