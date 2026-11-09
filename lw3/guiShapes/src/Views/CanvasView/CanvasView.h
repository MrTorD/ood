#pragma once

#include "Canvas.h"
#include <SFML/Graphics.hpp>

class CanvasView
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;

	CanvasView(EditorModel& model, sf::RenderWindow& window, const Bounds& canvasBounds)
		: m_view({ 2 * canvasBounds.x, 1.66f * canvasBounds.y }, { 1.5f * canvasBounds.w, 1.5f * canvasBounds.h })
		, m_window(window)
		, m_canvas(model, window, canvasBounds)
		, m_font("../../../lw3/guiShapes/assets/arial.ttf")
		, m_zoomInfo(m_font)
	{
		m_view.setViewport(sf::FloatRect({ 0.2, 0 }, { 0.8, 0.6 }));

		m_zoomInfo.setCharacterSize(40);
		m_zoomInfo.setFillColor(sf::Color::Black);
		m_zoomInfo.setPosition({ 0.79f * m_view.getSize().x, 0.67f * m_view.getSize().y });
		m_zoomInfo.setString("100%");

		m_zoomConnection = m_canvas.OnZoomChange([&](int newZoom) {
			m_zoomInfo.setString(std::format("{}%", newZoom));
		});
	}

	void Draw()
	{
		m_canvas.Draw();
		m_window.setView(m_view);
		m_window.draw(m_zoomInfo);
	}

	void HandleEvent(const sf::Event& event)
	{
		m_canvas.HandleEvent(event);
	}

private:
	sf::View m_view;
	sf::RenderWindow& m_window;
	Canvas m_canvas;
	ScopedConnection m_zoomConnection;
	sf::Font m_font;
	sf::Text m_zoomInfo;
};