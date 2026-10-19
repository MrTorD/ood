#pragma once

#include "CanvasController.h"
#include "CanvasView.h"
#include "PictureDocument.h"
#include "ToolBarController.h"
#include "ToolBarView.h"
#include <SFML/Graphics.hpp>

class EditorWindow
{
public:
	EditorWindow(PictureDocument& document)
		: m_window(sf::VideoMode({ 1000, 700 }, 16), "Picture")
		, m_document(document)
		, m_canvasBounds(150, 100, 700, 500)
		, m_model(
			  document.GetPicture(),
			  [&document](std::unique_ptr<ICommand> command) {
				  document.ExecuteCommand(std::move(command));
			  },
			  [&](std::unique_ptr<Picture> picture) {
				  document.SetPicture(std::move(picture));
				  m_model.SetPicture(document.GetPicture());
			  },
			  m_canvasBounds)
		, m_canvasView(m_model, m_canvasBounds)
		, m_toolBarView(m_model)
	{
	}

	void Run()
	{
		while (m_window.isOpen())
		{
			while (auto event = m_window.pollEvent())
			{
				HandleEvent(*event);
			}

			m_window.clear();
			m_canvasView.Draw(m_window);
			m_toolBarView.Draw(m_window);
			m_window.display();
		}
	}

private:
	sf::RenderWindow m_window;
	PictureDocument& m_document;

	Bounds m_canvasBounds;

	EditorModel m_model;
	CanvasView m_canvasView;
	ToolBarView m_toolBarView;

	void HandleEvent(const sf::Event& event)
	{
		if (event.is<sf::Event::Closed>())
		{
			m_window.close();
			return;
		}

		if (const auto* resized = event.getIf<sf::Event::Resized>())
		{
			sf::FloatRect visibleArea({ 0.f, 0.f }, sf::Vector2f(resized->size));
			m_window.setView(sf::View(visibleArea));
		}

		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Z && keyPressed->control)
			{
				m_document.Undo();
				return;
			}

			if (keyPressed->code == sf::Keyboard::Key::Y && keyPressed->control)
			{
				m_document.Redo();
				return;
			}
		}

		m_toolBarView.HandleEvent(event);
		m_canvasView.HandleEvent(event);
	}
};
