#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>

class Button
{
public:
	Button(
		const std::string& buttonText,
		std::function<void()> action,
		sf::Vector2f position,
		sf::Vector2f size,
		const sf::Font& font)
		: m_action(action)
		, m_buttonShape(size)
		, m_text(font)
	{
		m_buttonShape.setPosition(position);
		m_buttonShape.setFillColor(sf::Color::Cyan);

		m_text.setString(buttonText);
		m_text.setCharacterSize(16);
		m_text.setFillColor(sf::Color::Black);
		m_text.setPosition(position);
	}

	void Draw(sf::RenderTarget& target) const
	{
		target.draw(m_buttonShape);
		target.draw(m_text);
	}

	bool HitTest(sf::Vector2f point) const
	{
		return m_buttonShape
			.getGlobalBounds()
			.contains(point);
	}

	void Press() const
	{
		m_action();
	}

private:
	std::function<void()> m_action;
	sf::RectangleShape m_buttonShape;
	sf::Text m_text;
};