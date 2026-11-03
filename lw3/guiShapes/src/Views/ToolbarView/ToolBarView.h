#pragma once

#include "Button.h"
#include "ToolBarController.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

class ToolBarView
{
public:
	ToolBarView(EditorModel& model,
		const std::function<void()>& createNewWindow,
		const std::function<void(std::unique_ptr<Picture>)>& reopenWindow,
		const std::function<void()>& makeWindowCopy)
		: m_model(model)
		, m_controller(model, createNewWindow, reopenWindow, makeWindowCopy)
		, m_font(m_assetsPath / "arial.ttf")
		, m_view({ 55, 200 }, { 120, 400 })
	{
		m_buttons = GetButtonsConfiguration();
		m_view.setViewport(sf::FloatRect({ 0, 0 }, { 0.2, 0.6 }));
	}

	void Draw(sf::RenderTarget& target)
	{
		target.setView(m_view);

		DrawBackground(target);

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
			}
			else if (keyPressed->control)
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
				}
			}
		}
		else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			window.setView(m_view);
			auto viewPos = window.mapPixelToCoords(mousePressed->position);

			auto it = std::ranges::find_if(m_buttons,
				[viewPos](const Button& button) {
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

	std::filesystem::path m_assetsPath = "../../../lw3/guiShapes/assets/";

	sf::Font m_font;
	sf::View m_view;

	std::vector<Button> m_buttons;

	void DrawBackground(sf::RenderTarget& target) const
	{
		sf::RectangleShape drawable(m_view.getSize());

		drawable.setFillColor(sf::Color(0xF3F2F1FF));

		target.draw(drawable);
	}

	std::vector<Button> GetButtonsConfiguration()
	{
		return {
			{ "",
				[&]() {
					m_controller.SetCanvasMode(CanvasMode::SelectShapes);
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::SelectShapes;
				},
				sf::Vector2f(10, 10),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "cursor.png" },
			{ "",
				[&]() {
					m_controller.SetCanvasMode(CanvasMode::CreatingRectangle);
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::CreatingRectangle;
				},
				sf::Vector2f(10, 45),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "rectangle.png" },
			{ "",
				[&]() {
					m_controller.SetCanvasMode(CanvasMode::CreatingEllipse);
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::CreatingEllipse;
				},
				sf::Vector2f(10, 90),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "ellipse.png" },
			{ "",
				[&]() {
					m_controller.SetCanvasMode(CanvasMode::CreatingTriangle);
				},
				[&]() {
					return m_model.GetCanvasMode() == CanvasMode::CreatingTriangle;
				},
				sf::Vector2f(10, 130),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "triangle.png" },
			{ "Delete\nselected",
				[&]() {
					m_controller.DeleteSelectedShapes();
				},
				[&]() {
					return !m_model.GetSelectedShapes().empty();
				},
				sf::Vector2f(10, 170),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "cursor.png" },
			{ "Save to\ndocument",
				[&]() {
					m_controller.SaveToFile();
				},
				[]() { return true; },
				sf::Vector2f(10, 210),
				sf::Vector2f(30, 30),
				m_font, m_assetsPath / "cursor.png" },
			{ "Open from\nfile",
				[&]() {
					m_controller.OpenFromFile();
				},
				[]() { return true; },
				sf::Vector2f(10, 250),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "cursor.png" },
			{ "Make window\ncopy",
				[&]() {
					m_controller.MakeWindowCopy();
				},
				[]() { return true; },
				sf::Vector2f(10, 290),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "cursor.png" },
			{ "Create new\nwindow",
				[&]() {
					m_controller.CreateNewWindow();
				},
				[]() { return true; },
				sf::Vector2f(10, 330),
				sf::Vector2f(30, 30),
				m_font,
				m_assetsPath / "cursor.png" }
		};
	}
};