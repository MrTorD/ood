#pragma once

#include "TimelineController.h"
#include "TimelineRenderer.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

class TimelineView
{
public:
	TimelineView(EditorModel& model)
		: m_model(model)
		, m_view({ 225, 300 }, { 500, 600 })
		, m_controller(model, m_shapeBoxWidth, m_shapeBoxHeight, m_shapeBoxPadding)
	{
		m_view.setViewport(sf::FloatRect({ 0, 0.6 }, { 1, 0.4 }));
	}

	void HandleEvent(const sf::Event& event, sf::RenderTarget& target)
	{
		target.setView(m_view);

		if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			auto pos = NormalizeMousePos(target, mousePressed->position);

			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)
				? m_controller.OnMouseWithCtrlPressed(pos)
				: m_controller.OnMousePressed(pos);
		}
		else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
		{
			auto pos = NormalizeMousePos(target, mouseMoved->position);
			m_controller.OnMouseMoved(pos);
		}
		else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
		{
			auto pos = NormalizeMousePos(target, mouseReleased->position);
			m_controller.OnMouseReleased(pos);
		}
		else if (const auto* buttonPressed = event.getIf<sf::Event::KeyPressed>())
		{
			OnKeyPressed(buttonPressed->code);
		}
	}

	void Draw(sf::RenderTarget& target)
	{
		target.setView(m_view);
		TimelineRenderer renderer(target, m_shapeBoxWidth, m_shapeBoxHeight, m_shapeBoxPadding);
		renderer.Draw(m_model);
	}

private:
	EditorModel& m_model;
	sf::View m_view;

	float m_zoomStep = 0.9;
	float m_moveStep = 25;
	float m_shapeBoxHeight = 50;
	float m_shapeBoxWidth = 200;
	float m_shapeBoxPadding = 15;

	TimelineController m_controller;

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
		case sf::Keyboard::Key::Left:
			m_view.move({ -m_moveStep, 0 });
			return;
		case sf::Keyboard::Key::Right:
			m_view.move({ m_moveStep, 0 });
			return;
		case sf::Keyboard::Key::Up:
			m_view.move({ 0, -m_moveStep });
			return;
		case sf::Keyboard::Key::Down:
			m_view.move({ 0, m_moveStep });
			return;
		}
	}

	Point NormalizeMousePos(const sf::RenderTarget& target, sf::Vector2i position)
	{
		return ConvertToPoint(target.mapPixelToCoords(position));
	}
};