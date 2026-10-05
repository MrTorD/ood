#pragma once

#include "CanvasController.h"
#include "Picture.h"
#include "PictureRenderer.h"
#include "SfVectorToPoint.h"
#include <SFML/Window/Event.hpp>

class CanvasView
{
public:
	CanvasView(Picture& picture, EditorSession& session, sf::RenderTarget& target)
		: m_picture(picture)
		, m_controller(picture, session)
		, m_session(session)
		, m_renderer(target)
	{
	}

	void HandleEvent(sf::Event& event)
	{
		if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			m_controller.OnMousePressed(ConvertToPoint(pressed->position));
		}
		else if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
		{
			m_controller.OnMouseReleased(ConvertToPoint(released->position));
		}
		else if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
		{
			m_controller.OnMouseMoved(ConvertToPoint(moved->position));
		}
	}

	void Draw()
	{
		m_renderer.Draw(m_picture, m_session.GetSelectedShape());
	}

private:
	Picture& m_picture;
	CanvasController m_controller;
	const EditorSession& m_session;

	PictureRenderer m_renderer;
};