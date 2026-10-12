#pragma once

#include "Button.h"
#include "Picture.h"
#include "ToolBarController.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

class ToolBarView
{
public:
	ToolBarView(Picture& picture, sf::RenderTarget& target, EditorSession& session)
		: m_controller(picture, session)
		, m_target(target)
		, m_font("arial.ttf")
	{
		m_buttons.emplace_back(
			"Add rectangle",
			[&]() {
				m_controller.AddRectangle();
			},
			sf::Vector2f(10, 10),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Add ellipse",
			[&]() {
				m_controller.AddEllipse();
			},
			sf::Vector2f(10, 60),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Add triangle",
			[&]() {
				m_controller.AddTriangle();
			},
			sf::Vector2f(10, 110),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Delete selected\nshape",
			[&]() {
				m_controller.DeleteSelectedShape();
			},
			sf::Vector2f(10, 160),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Save to\ndocument",
			[&]() {
				m_controller.SaveToDocument();
			},
			sf::Vector2f(10, 210),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Load from\ndocument",
			[&]() {
				m_controller.LoadFromDocument();
			},
			sf::Vector2f(10, 260),
			sf::Vector2f(110, 40),
			m_font);
	}

	void Draw()
	{
		for (const auto& button : m_buttons)
		{
			button.Draw(m_target);
		}
	}

	void HandleEvent(sf::Event& event)
	{
		if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			auto it = std::find_if(
				m_buttons.begin(),
				m_buttons.end(),
				[&pressed](const Button& button) {
					return button.HitTest(static_cast<sf::Vector2f>(pressed->position));
				});

			if (it != m_buttons.end())
			{
				it->Press();
			}
		}
	}

private:
	ToolBarController m_controller;
	sf::RenderTarget& m_target;
	sf::Font m_font;

	std::vector<Button> m_buttons;
};
