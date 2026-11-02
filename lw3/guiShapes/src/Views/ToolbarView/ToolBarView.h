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
		: m_model(model)
		, m_controller(model, std::move(createNewWindow), std::move(reopenWindow), std::move(makeWindowCopy))
		, m_font("arial.ttf")
		, m_view({ 55, 200 }, { 120, 400 })
	{
		m_buttons = GetButtonsConfiguration();
		m_view.setViewport(sf::FloatRect({ 0, 0 }, { 0.2, 0.6 }));
	}

	void Draw(sf::RenderTarget& target)
	{
		target.setView(m_view);

		for (auto& button : m_buttons)
		{
			button.Draw(target);
		}
	}

	void HandleEvent(const sf::Event& event, sf::RenderWindow& window)
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
			window.setView(m_view);
			auto viewPos = window.mapPixelToCoords(mousePressed->position);

			auto it = std::find_if(
				m_buttons.begin(),
				m_buttons.end(),
				[&mousePressed, viewPos](const Button& button) {
					return button.IsContains(viewPos);
				});

			if (it != m_buttons.end())
			{
				it->Press();
			}
		}
	}

private:
	EditorModel& m_model;
	ToolBarController m_controller;
	sf::Font m_font;
	sf::View m_view;

	std::vector<Button> m_buttons;

	std::vector<Button> GetButtonsConfiguration()
	{
		return {
			{ "Select mode",
				[&]() {
					m_controller.SetSelectMode();
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::SelectShapes;
				},
				sf::Vector2f(10, 10),
				sf::Vector2f(100, 35),
				m_font },
			{ "Rectangle\nmode",
				[&]() {
					m_controller.SetRectangleMode();
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::CreatingRectangle;
				},
				sf::Vector2f(10, 50),
				sf::Vector2f(100, 35),
				m_font },
			{ "Ellipse\nmode",
				[&]() {
					m_controller.SetEllipseMode();
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::CreatingEllipse;
				},
				sf::Vector2f(10, 90),
				sf::Vector2f(100, 35),
				m_font },
			{ "Triangle\nmode",
				[&]() {
					m_controller.SetTriangleMode();
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::CreatingTriangle;
				},
				sf::Vector2f(10, 130),
				sf::Vector2f(100, 35),
				m_font },
			{ "Delete\nselected",
				[&]() {
					m_controller.DeleteSelectedShapes();
				},
				[&]() {
					return !m_model.GetSelectedShapes().empty();
				},
				sf::Vector2f(10, 170),
				sf::Vector2f(100, 35),
				m_font },
			{ "Save to\ndocument",
				[&]() {
					m_controller.SaveToFile();
				},
				[]() { return true; },
				sf::Vector2f(10, 210),
				sf::Vector2f(100, 35),
				m_font },
			{ "Open from\nfile",
				[&]() {
					m_controller.OpenFromFile();
				},
				[]() { return true; },
				sf::Vector2f(10, 250),
				sf::Vector2f(100, 35),
				m_font },
			{ "Make window\ncopy",
				[&]() {
					m_controller.MakeWindowCopy();
				},
				[]() { return true; },
				sf::Vector2f(10, 290),
				sf::Vector2f(100, 35),
				m_font },
			{ "Create new\nwindow",
				[&]() {
					m_controller.CreateNewWindow();
				},
				[]() { return true; },
				sf::Vector2f(10, 330),
				sf::Vector2f(100, 35),
				m_font }
		};
	}
};