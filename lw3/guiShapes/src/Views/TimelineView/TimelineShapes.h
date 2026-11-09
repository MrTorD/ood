#pragma once

#include "TimelineController.h"
#include "TimelineRenderer.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

class TimelineShapes
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;
	using ChangeSignal = boost::signals2::signal<void(int)>;

	TimelineShapes(EditorModel& model, sf::RenderTarget& target)
		: m_model(model)
		, m_target(target)
		, m_controller(model)
		, m_renderer(target)
		, m_view({ 300, 150 }, { 600, 300 })
		, m_zoomHandler(m_view)
		, m_background({ 2000, 2000 })
		, m_timeArrow({ 2.f, 2000 })
	{
		m_view.setViewport(sf::FloatRect({ 0.2, 0.67 }, { 0.8, 0.33 }));
		m_background.setFillColor(sf::Color(0xF3F3F3FF));

		m_timeArrow.setFillColor(sf::Color::Black);
		m_timeArrow.setPosition({ m_model.GetCurrentTime(), 0 });
	}

	void Draw()
	{
		m_target.setView(m_view);
		m_target.draw(m_background);
		m_renderer.Draw(m_model);
		m_timeArrow.setPosition({ m_model.GetCurrentTime(), 0 });
		m_target.draw(m_timeArrow);
	}

	void HandleEvent(const sf::Event& event)
	{
		m_target.setView(m_view);

		if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			auto pos = NormalizeMousePos(m_target, mousePressed->position);

			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)
				? m_controller.OnMouseWithCtrlPressed(pos)
				: m_controller.OnMousePressed(pos);
		}
		else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
		{
			auto pos = NormalizeMousePos(m_target, mouseMoved->position);
			m_controller.OnMouseMoved(pos);
		}
		else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
		{
			auto pos = NormalizeMousePos(m_target, mouseReleased->position);
			m_controller.OnMouseReleased(pos);
		}
		else if (const auto* buttonPressed = event.getIf<sf::Event::KeyPressed>())
		{
			OnKeyPressed(buttonPressed->code);
		}
	}

	ScopedConnection OnZoomChange(const ChangeSignal::slot_type& slot)
	{
		return m_zoomHandler.DoOnChange(slot);
	}

private:
	float m_moveStepX = 10;
	float m_moveStepY = 25;

	EditorModel& m_model;
	sf::RenderTarget& m_target;
	TimelineController m_controller;
	TimelineRenderer m_renderer;
	sf::View m_view;
	ZoomHandler m_zoomHandler;
	sf::RectangleShape m_background;
	sf::RectangleShape m_timeArrow;

	void OnKeyPressed(sf::Keyboard::Key key)
	{
		switch (key)
		{
		case sf::Keyboard::Key::Equal:
			m_zoomHandler.Unzoom();
			return;
		case sf::Keyboard::Key::Hyphen:
			m_zoomHandler.Zoom();
			return;
		case sf::Keyboard::Key::Num0:
			m_view.setCenter({ 300.f, 150.f });
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
};