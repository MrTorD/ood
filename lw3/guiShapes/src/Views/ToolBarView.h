#pragma once

#include "Button.h"
#include "ToolBarController.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

class ToolBarView
{
public:
	explicit ToolBarView(EditorModel& model)
		: m_controller(model)
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
			"Delete selected\nshapes",
			[&]() {
				m_controller.DeleteSelectedShapes();
			},
			sf::Vector2f(10, 160),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Save to\ndocument",
			[&]() {
				m_controller.SaveToFile();
			},
			sf::Vector2f(10, 210),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Load from\ndocument",
			[&]() {
				m_controller.LoadFromFile();
			},
			sf::Vector2f(10, 260),
			sf::Vector2f(110, 40),
			m_font);
	}

	void Draw(sf::RenderTarget& target)
	{
		for (const auto& button : m_buttons)
		{
			button.Draw(target);
		}
	}

	void HandleEvent(const sf::Event& event)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete))
		{
			m_controller.DeleteSelectedShapes();
		}

		if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			auto it = std::find_if(
				m_buttons.begin(),
				m_buttons.end(),
				[&pressed](const Button& button) {
					return button.IsContains(static_cast<sf::Vector2f>(pressed->position));
				});

			if (it != m_buttons.end())
			{
				it->Press();
			}
		}
	}

private:
	ToolBarController m_controller;
	sf::Font m_font;

	std::vector<Button> m_buttons;
};
