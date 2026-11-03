#pragma once

#include "CanvasController.h"
#include "PictureRenderer.h"
#include "SfVectorToPoint.h"
#include "ZoomHandler.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

class CanvasView
{
	// [ ] Поправить общий UX приложения
	// [x] Сделать так, чтобы после создания фигуры приложение возвращалось в обычный режим выделения
	// [ ] Выводить текущий уровень зума, добавить его максимальный уровень и возможность сброса
public:
	CanvasView(EditorModel& model, sf::RenderTarget& target, const Bounds& canvasBounds)
		: m_model(model)
		, m_target(target)
		, m_renderer(target)
		, m_canvasBounds(canvasBounds)
		, m_controller(model)
		, m_view({ m_canvasBounds.x, m_canvasBounds.y }, { m_canvasBounds.w, m_canvasBounds.h })
		, m_zoomHandler(m_view)
		, m_background({m_canvasBounds.w, m_canvasBounds.h})
	{
		m_view.setViewport(sf::FloatRect({ 0.2, 0 }, { 0.825, 0.6 }));
		m_view.move(m_view.getCenter());
		m_view.zoom(1.5);

		m_background.setFillColor(sf::Color::White);
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

	void Draw()
	{
		m_target.setView(m_view);
		m_target.draw(m_background);
		m_renderer.Draw(m_model);
	}

private:
	EditorModel& m_model;
	sf::RenderTarget& m_target;
	PictureRenderer m_renderer;
	Bounds m_canvasBounds;
	CanvasController m_controller;
	sf::View m_view;
	ZoomHandler m_zoomHandler;
	sf::RectangleShape m_background;

	float m_moveStep = 25.f;

	void OnKeyPressed(sf::Keyboard::Key key)
	{
		switch (key)
		{
		case sf::Keyboard::Key::Num2:
			m_zoomHandler.Zoom();
			return;
		case sf::Keyboard::Key::Num1:
			m_zoomHandler.Unzoom();
			return;
		case sf::Keyboard::Key::R:
			m_zoomHandler.ResetZoom();
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

	// TODO: Вынести??
	static Point NormalizeMousePos(const sf::RenderTarget& target, sf::Vector2i position)
	{
		return ConvertToPoint(target.mapPixelToCoords(position));
	}
};