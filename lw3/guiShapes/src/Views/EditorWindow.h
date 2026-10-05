#pragma once

#include "CanvasView.h"
#include "Picture.h"
#include "ToolBarView.h"
#include <SFML/Graphics.hpp>

class EditorWindow
{

public:
	explicit EditorWindow(Picture& picture)
		: m_window(sf::VideoMode({ 800, 600 }), "Picture")
		, m_canvasView(picture, m_session, m_window)
		, m_toolBarView(picture, m_window, m_session)
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
	EditorSession m_session;

	sf::RenderWindow m_window;
	CanvasView m_canvasView;
	ToolBarView m_toolBarView;
};
