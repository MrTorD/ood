#pragma once

#include "CanvasController.h"
#include "CanvasView.h"
#include "PictureDocument.h"
#include "TimelineView.h"
#include "ToolBarController.h"
#include "ToolBarView.h"
#include <SFML/Graphics.hpp>

class EditorWindow
{
public:
	EditorWindow(
		PictureDocument& document,
		std::function<void(EditorWindow*)> close,
		std::function<void(std::unique_ptr<Picture>)> addSeparateWindow,
		std::function<void(PictureDocument&)> makeWindowCopy)
		: m_window(sf::VideoMode({ 1000, 700 }, 16), "Picture")
		, m_document(document)
		, m_close(std::move(close))
		, m_canvasBounds(250, 150, 600, 450)
		, m_model(
			  document.GetPicture(),
			  [&document](std::unique_ptr<ICommand> command) {
				  document.ExecuteCommand(std::move(command));
			  },
			  m_canvasBounds)
		, m_canvasView(m_model, m_canvasBounds)
		, m_toolBarView(
			  m_model,
			  [addSeparateWindow]() {
				  addSeparateWindow(std::make_unique<Picture>());
			  },
			  [this, addSeparateWindow](std::unique_ptr<Picture> pic) {
				  addSeparateWindow(std::move(pic));
				  m_close(this);
			  },
			  [this, makeWindowCopy]() {
				  makeWindowCopy(m_document);
			  })
		, m_timelineView(m_model)
	{
	}

	void Render()
	{
		while (auto event = m_window.pollEvent())
		{
			HandleEvent(*event);
		}

		m_window.clear();
		m_canvasView.Draw(m_window);
		m_toolBarView.Draw(m_window);
		m_timelineView.Draw(m_window);
		m_window.display();
	}

private:
	sf::RenderWindow m_window;
	PictureDocument& m_document;
	std::function<void(EditorWindow*)> m_close;

	Bounds m_canvasBounds;

	EditorModel m_model;
	CanvasView m_canvasView;
	ToolBarView m_toolBarView;
	TimelineView m_timelineView;

	void HandleEvent(const sf::Event& event)
	{
		if (event.is<sf::Event::Closed>())
		{
			m_close(this);
			m_window.close();
			return;
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

		m_toolBarView.HandleEvent(event, m_window);
		m_canvasView.HandleEvent(event, m_window);
		m_timelineView.HandleEvent(event, m_window);
	}
};
