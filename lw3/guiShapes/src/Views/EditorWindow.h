#pragma once

#include "CanvasView.h"
#include "ToolBarView.h"
#include "CanvasController.h"
#include "ToolBarController.h"
#include <SFML/Graphics.hpp>

class EditorWindow
{
public:
	EditorWindow(CanvasController& canvasController, ToolBarController& toolBarContoller, EditorSession& session)
		: m_window(sf::VideoMode({ 800, 600 }, 16), "Picture")
		, m_canvasView(canvasController, m_window, session)
		, m_toolBarView(toolBarContoller, m_window)
	{
	}

	void Run()
	{
		while (m_window.isOpen())
		{
			while (auto event = m_window.pollEvent())
			{
				if (!event->is<sf::Event::Closed>())
				{
					m_toolBarView.HandleEvent(*event);
					m_canvasView.HandleEvent(*event);
				}
				else
				{
					m_window.close();
				}
			}

			m_window.clear();
			m_canvasView.Draw();
			m_toolBarView.Draw();
			m_window.display();
		}
	}

private:
	sf::RenderWindow m_window;
	CanvasView m_canvasView;
	ToolBarView m_toolBarView;
};
