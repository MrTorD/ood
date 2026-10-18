#pragma once

#include "CanvasController.h"
#include "PictureRenderer.h"
#include "SfVectorToPoint.h"
#include <SFML/Window/Event.hpp>

class CanvasView
{
public:
	CanvasView(EditorModel& model, Bounds canvasBounds)
		: m_model(model)
		, m_canvasBounds(canvasBounds)
		, m_controller(model, canvasBounds)
	{
	}

	void HandleEvent(const sf::Event& event)
	{
		if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)
				? m_controller.OnMouseWithCtrlPressed(ConvertToPoint(pressed->position))
				: m_controller.OnMousePressed(ConvertToPoint(pressed->position));
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

	void Draw(sf::RenderTarget& target)
	{
		DrawCanvasBounds(target);
		PictureRenderer renderer(target, { m_canvasBounds.x, m_canvasBounds.y });
		renderer.Draw(m_model);
	}

private:
	EditorModel& m_model;
	Bounds m_canvasBounds;
	CanvasController m_controller;

	void DrawCanvasBounds(sf::RenderTarget& target)
	{
		auto [x, y, w, h] = m_canvasBounds;

		sf::RectangleShape drawable({ w, h });

		drawable.setPosition({ x, y });
		drawable.setFillColor(sf::Color::Transparent);
		drawable.setOutlineThickness(5);
		drawable.setOutlineColor(sf::Color::White);

		target.draw(drawable);
	}
};