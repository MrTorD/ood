#pragma once

#include "Button.h"
#include "ToolBarController.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

class ToolBarView
{
public:
	ToolBarView(EditorModel& model,
		std::function<void()> createNewWindow,
		std::function<void(std::unique_ptr<Picture>)> reopenWindow,
		std::function<void()> makeWindowCopy)
		: m_controller(model, std::move(createNewWindow), std::move(reopenWindow), std::move(makeWindowCopy))
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
			"Open from\nfile",
			[&]() {
				m_controller.OpenFromFile();
			},
			sf::Vector2f(10, 260),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Make window\ncopy",
			[&]() {
				m_controller.MakeWindowCopy();
			},
			sf::Vector2f(10, 310),
			sf::Vector2f(110, 40),
			m_font);
		m_buttons.emplace_back(
			"Create new\nwindow",
			[&]() {
				m_controller.CreateNewWindow();
			},
			sf::Vector2f(10, 360),
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
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Delete)
			{
				m_controller.DeleteSelectedShapes();
				return;
			}

			if (keyPressed->control)
			{
				switch (keyPressed->code)
				{
				case sf::Keyboard::Key::S:
					m_controller.SaveToFile();
					return;
				case sf::Keyboard::Key::N:
					m_controller.CreateNewWindow();
					return;
				case sf::Keyboard::Key::O:
					m_controller.OpenFromFile();
					return;
				default:
					return;
				}
			}
		}
		else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			auto it = std::find_if(
				m_buttons.begin(),
				m_buttons.end(),
				[&mousePressed](const Button& button) {
					return button.IsContains(static_cast<sf::Vector2f>(mousePressed->position));
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
