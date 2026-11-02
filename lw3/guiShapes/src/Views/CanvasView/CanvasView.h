#pragma once

#include "CanvasController.h"
#include "PictureRenderer.h"
#include "SfVectorToPoint.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

class CanvasView
{
public:
	CanvasView(EditorModel& model, Bounds& canvasBounds)
		: m_model(model)
		, m_canvasBounds(canvasBounds)
		, m_controller(model)
		, m_view({ m_canvasBounds.x, m_canvasBounds.y }, { m_canvasBounds.w, m_canvasBounds.h })
	{
		m_view.setViewport(sf::FloatRect({ 0.2, 0 }, { 0.825, 0.6 }));
		m_view.zoom(2);
	}

	void HandleEvent(const sf::Event& event, sf::RenderWindow& window)
	{
		window.setView(m_view);
		if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)
				? m_controller.OnMouseWithCtrlPressed(NormalizeMousePos(window, pressed->position))
				: m_controller.OnMousePressed(NormalizeMousePos(window, pressed->position));
		}
		else if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
		{
			m_controller.OnMouseReleased(NormalizeMousePos(window, released->position));
		}
		else if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
		{
			m_controller.OnMouseMoved(NormalizeMousePos(window, moved->position));
		}
		else if (event.is<sf::Event::FocusLost>())
		{
			auto pos = sf::Mouse::getPosition(window);

			m_controller.OnMouseReleased(NormalizeMousePos(window, pos));
		}
		else if (const auto* buttonPressed = event.getIf<sf::Event::KeyPressed>())
		{
			OnKeyPressed(buttonPressed->code);
		}
	}

	void Draw(sf::RenderTarget& target)
	{
		target.setView(m_view);
		DrawCanvasBounds(target);
		PictureRenderer renderer(target);
		renderer.Draw(m_model);
	}

private:
	EditorModel& m_model;
	Bounds& m_canvasBounds;
	sf::View m_view;
	CanvasController m_controller;
	float m_zoomStep = 0.9f;
	float m_moveStep = 25.f;

	void DrawCanvasBounds(sf::RenderTarget& target)
	{
		auto [x, y, w, h] = m_canvasBounds;

		sf::RectangleShape drawable({ w, h });

		drawable.setFillColor(sf::Color::Transparent);
		drawable.setOutlineThickness(5);
		drawable.setOutlineColor(sf::Color::White);

		target.draw(drawable);
	}

	void OnKeyPressed(sf::Keyboard::Key key)
	{
		switch (key)
		{
		case sf::Keyboard::Key::Equal:
			m_view.zoom(m_zoomStep);
			return;
		case sf::Keyboard::Key::Hyphen:
			m_view.zoom(1 / m_zoomStep);
			return;
		case sf::Keyboard::Key::D:
			m_view.move({ m_moveStep, 0 });
			return;
		case sf::Keyboard::Key::A:
			m_view.move({ -m_moveStep, 0 });
			return;
		case sf::Keyboard::Key::W:
			m_view.move({ 0, -m_moveStep });
			return;
		case sf::Keyboard::Key::S:
			m_view.move({ 0, m_moveStep });
			return;
		}
	}

	Point NormalizeMousePos(const sf::RenderTarget& target, sf::Vector2i position)
	{
		return ConvertToPoint(target.mapPixelToCoords(position));
	}
};