#pragma once

#include "Picture.h"
#include "SfVectorToPoint.h"
#include "ToolBarController.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <memory>

class ToolBarView
{
public:
	ToolBarView(Picture& picture, sf::RenderTarget& target, EditorSession& session)
		: m_picture(picture)
		, m_controller(picture, session)
		, m_target(target)
		, m_font("arial.ttf")
	{
	}

	void Draw()
	{
		for (size_t i = 0; i < m_controller.GetButtonsCount(); i++)
		{
			const auto& button = m_controller.GetButton(i);
			auto bounds = button.GetBounds();

			DrawButton(bounds, button.GetColor());
			DrawText(bounds, button.GetText());
		}
	}

	void HandleEvent(sf::Event& event)
	{
		if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			m_controller.OnMousePressed(ConvertToPoint(pressed->position));
		}
	}

private:
	Picture& m_picture;
	ToolBarController m_controller;
	sf::RenderTarget& m_target;
	sf::Font m_font;

	void DrawButton(Bounds bounds, Color color)
	{
		auto [x, y, w, h] = bounds;

		sf::RectangleShape drawableButton({ w, h });

		drawableButton.setPosition({ x, y });
		drawableButton.setFillColor(sf::Color(color));

		m_target.draw(drawableButton);
	}

	void DrawText(Bounds bounds, const std::string& text)
	{
		auto [x, y, w, h] = bounds;

		sf::Text drawableText(m_font);

		drawableText.setString(text);
		drawableText.setCharacterSize(16);
		drawableText.setFillColor(sf::Color::Black);

		drawableText.setPosition({ x, y });

		m_target.draw(drawableText);
	}
};
