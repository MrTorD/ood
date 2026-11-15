#pragma once

#include "CanvasController.h"
#include "ConvertFromSf.h"
#include "PictureRenderer.h"
#include "ZoomHandler.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

class Canvas
{
	// [ ] Поправить общий UX приложения
	// [x] Сделать так, чтобы после создания фигуры приложение возвращалось в обычный режим выделения
	// [ ] Выводить текущий уровень зума, добавить его максимальный уровень и возможность сброса
public:
	using ScopedConnection = boost::signals2::scoped_connection;
	using ChangeSignal = boost::signals2::signal<void(int)>;

	Canvas(EditorModel& model, sf::RenderWindow& window, const Bounds& canvasBounds)
		: m_model(model)
		, m_window(window)
		, m_canvasBounds(canvasBounds)
		, m_renderer(window)
		, m_controller(model)
		, m_view({ 2 * m_canvasBounds.x, 2 * m_canvasBounds.y }, { 1.5f * canvasBounds.w, 1.5f * canvasBounds.h })
		, m_zoomHandler(m_view)
		, m_background({ canvasBounds.w, canvasBounds.h })
	{
		m_view.setViewport(sf::FloatRect({ 0.2, 0 }, { 0.8, 0.6 }));

		m_background.setFillColor(sf::Color::White);
	}

	void HandleEvent(const sf::Event& event)
	{
		m_window.setView(m_view);
		if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)
				? m_controller.OnMouseWithCtrlPressed(NormalizeMousePos(m_window, pressed->position))
				: m_controller.OnMousePressed(NormalizeMousePos(m_window, pressed->position));
		}
		else if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
		{
			m_controller.OnMouseReleased(NormalizeMousePos(m_window, released->position));
		}
		else if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
		{
			m_controller.OnMouseMoved(NormalizeMousePos(m_window, moved->position));
		}
		else if (event.is<sf::Event::FocusLost>())
		{
			auto pos = sf::Mouse::getPosition(m_window);

			m_controller.OnMouseReleased(NormalizeMousePos(m_window, pos));
		}
		else if (const auto* buttonPressed = event.getIf<sf::Event::KeyPressed>())
		{
			OnKeyPressed(buttonPressed->code);
		}
	}

	void Draw()
	{
		m_window.setView(m_view);
		m_window.draw(m_background);
		m_renderer.Draw(m_model);
	}

	ScopedConnection OnZoomChange(const ChangeSignal::slot_type& slot)
	{
		return m_zoomHandler.DoOnChange(slot);
	}

private:
	EditorModel& m_model;
	sf::RenderWindow& m_window;
	Bounds m_canvasBounds;
	PictureRenderer m_renderer;
	CanvasController m_controller;
	sf::View m_view;
	ZoomHandler m_zoomHandler;
	sf::RectangleShape m_background;

	float m_moveStep = 25.f;

	void OnKeyPressed(sf::Keyboard::Key key)
	{
		switch (key)
		{
		case sf::Keyboard::Key::Num1:
			m_zoomHandler.Unzoom();
			return;
		case sf::Keyboard::Key::Num2:
			m_zoomHandler.Zoom();
			return;
		case sf::Keyboard::Key::R:
			m_zoomHandler.ResetZoom();
			m_view.setCenter({ 2 * m_canvasBounds.x, 2 * m_canvasBounds.y });
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
		case sf::Keyboard::Key::G:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
			{
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)
					? m_controller.OnUngroupPressed()
					: m_controller.OnGroupPressed();
			}
		}
	}
};