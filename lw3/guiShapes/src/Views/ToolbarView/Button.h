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
		std::function<bool()> checkIsActive,
		sf::Vector2f position,
		sf::Vector2f size,
		const sf::Font& font,
		const std::filesystem::path& imagePath)
		: m_action(std::move(action))
		, m_checkIsActive(std::move(checkIsActive))
		, m_buttonShape(size)
		, m_text(font)
		, m_texture(imagePath)
		, m_sprite(m_texture)
	{

		// TODO: Упростить, разобраться с текстурами и спрайтами (проверить, обязательно ли нужно каждый раз setTexture заново)
		// TODO: Возможно добавить несколько наследников-реализаций Button, одна работает с текстом, другая с картинками
		m_sprite.setPosition(position);
		m_sprite.setScale({ 0.5, 0.5 });

		m_text.setString(buttonText);
		m_text.setCharacterSize(8);
		m_text.setFillColor(sf::Color::Black);
		m_text.setPosition(position);

		m_buttonShape.setPosition(position);
		m_buttonShape.setOutlineThickness(2);
	}

	void Draw(sf::RenderTarget& target)
	{
		m_buttonShape.setFillColor(m_checkIsActive()
			? sf::Color(0xF3F3F3FF)
			: sf::Color::White);

		m_buttonShape.setOutlineColor(m_checkIsActive() ? sf::Color(0x808080FF) : sf::Color::White);

		target.draw(m_buttonShape);
		target.draw(m_text);
		m_sprite.setTexture(m_texture);
		target.draw(m_sprite);
	}

	bool IsContains(sf::Vector2f point) const
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
	std::function<bool()> m_checkIsActive;
	sf::RectangleShape m_buttonShape;
	sf::Text m_text;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};