#pragma once

#include "TimelineController.h"
#include "TimelineRenderer.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

class TimelineView
{
public:
	TimelineView(EditorModel& model, sf::RenderTarget& target)
		: m_model(model)
		, m_target(target)
		, m_controller(model, m_shapeBoxWidth, m_shapeBoxHeight, m_shapeBoxPadding)
		, m_renderer(target, m_shapeBoxWidth, m_shapeBoxHeight, m_shapeBoxPadding)
		, m_view({ 150, 150 }, { 300, 300 })
		, m_zoomHandler(m_view)
		, m_background(m_view.getSize())
	{
		m_view.setViewport(sf::FloatRect({ 0, 0.6 }, { 1, 0.4 }));
		m_background.setFillColor(sf::Color(0xF3F3F3FF));
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

	void Draw()
	{
		m_target.setView(m_view);
		m_target.draw(m_background);
		m_renderer.Draw(m_model);
	}

private:
	float m_moveStepX = 10;
	float m_moveStepY = 25;
	float m_shapeBoxHeight = 50;
	float m_shapeBoxWidth = 200;
	float m_shapeBoxPadding = 15;

	EditorModel& m_model;
	sf::RenderTarget& m_target;
	TimelineRenderer m_renderer;
	TimelineController m_controller;
	sf::View m_view;
	ZoomHandler m_zoomHandler;
	sf::RectangleShape m_background;

	void OnKeyPressed(sf::Keyboard::Key key)
	{
		switch (key)
		{
		case sf::Keyboard::Key::Equal:
			m_zoomHandler.Zoom();
			return;
		case sf::Keyboard::Key::Hyphen:
			m_zoomHandler.Unzoom();
			return;
		case sf::Keyboard::Key::Num0:
			m_zoomHandler.ResetZoom();
			return;
		case sf::Keyboard::Key::Left:
			m_view.move({ -m_moveStepX, 0 });
			return;
		case sf::Keyboard::Key::Right:
			m_view.move({ m_moveStepX, 0 });
			return;
		case sf::Keyboard::Key::Up:
			m_view.move({ 0, -m_moveStepY });
			return;
		case sf::Keyboard::Key::Down:
			m_view.move({ 0, m_moveStepY });
		}
	}

	static Point NormalizeMousePos(const sf::RenderTarget& target, sf::Vector2i position)
	{
		return ConvertToPoint(target.mapPixelToCoords(position));
	}
};