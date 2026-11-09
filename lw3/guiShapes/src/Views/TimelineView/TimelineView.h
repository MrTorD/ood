#pragma once

#include "Button.h"
#include "EditorModel.h"
#include "TimelineShapes.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

class TimelineView
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;

	TimelineView(EditorModel& model, sf::RenderTarget& target)
		: m_model(model)
		, m_target(target)
		, m_view({ 500, 50 }, { 1000, 100 })
		, m_timelineShapes(model, target)
		, m_assetsPath("../../../lw3/guiShapes/assets/")
		, m_font(m_assetsPath / "arial.ttf")
		, m_pauseButton(
			  "",
			  []() {},
			  []() { return false; },
			  { 0.5f * m_view.getSize().x, 0.3f * m_view.getSize().y },
			  { 60, 60 },
			  m_font, // TODO: Temp
			  m_assetsPath / "pause.png")
		, m_zoomInfo(m_font)
	{
		m_view.setViewport(sf::FloatRect({ 0.2, 0.6 }, { 0.8, 0.07 }));
		m_background.setFillColor(sf::Color(0x6ac8e1FF));
		m_background.setSize({ m_view.getSize().x, m_view.getSize().y });

		// TODO: TEMP
		m_zoomInfo.setCharacterSize(40);
		m_zoomInfo.setFillColor(sf::Color::Black);
		m_zoomInfo.setPosition({ 0.8f * m_view.getSize().x, 0.25f * m_view.getSize().y });

		m_zoomInfo.setString("100%");

		m_zoomConnection = m_timelineShapes.OnZoomChange([&](int newZoom) {
			m_zoomInfo.setString(std::format("{}%", newZoom));
		});
	}

	void Draw()
	{
		m_target.setView(m_view);
		// TODO:
		// m_background.setSize({m_view.getSize()}); // TODO: Подумать нормально ли?
		// m_background.setPosition(m_view.getCenter());
		m_target.draw(m_background);
		m_timelineShapes.Draw();
		m_target.setView(m_view);
		m_pauseButton.Draw(m_target);

		m_target.draw(m_zoomInfo);
	}

	void HandleEvent(const sf::Event& event)
	{
		m_timelineShapes.HandleEvent(event);
	}

private:
	EditorModel& m_model;
	sf::RenderTarget& m_target;
	sf::View m_view;
	TimelineShapes m_timelineShapes;

	std::filesystem::path m_assetsPath; // TODO: Подумать о выносе в какое-то глобальное супер пространство?
	sf::Font m_font;
	Button m_pauseButton;
	sf::Text m_zoomInfo;
	sf::RectangleShape m_background;

	ScopedConnection m_zoomConnection;
};