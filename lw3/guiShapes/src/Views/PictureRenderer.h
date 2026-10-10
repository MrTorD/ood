#pragma once

#include "Ellipse.h"
#include "IShapeOperation.h"
#include "Picture.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class PictureRenderer : public IShapeOperation
{
public:
//TODO: Избавиться от зависимости View ot Model
	explicit PictureRenderer(sf::RenderTarget& target)
		: m_target(target)
	{
	}

	void Draw(Picture& picture, const Shape* selectedShape)
	{
		for (size_t i = 0; i < picture.GetShapesCount(); i++)
		{
			picture.GetShape(i).ApplyOperation(*this);
		}

		if (selectedShape)
		{
			auto [x, y, w, h] = selectedShape->GetBounds();
			sf::RectangleShape selection({ w, h });

			selection.setPosition({ x, y });
			selection.setOutlineColor(sf::Color(m_selectionColor));
			selection.setOutlineThickness(m_selectionThickness);
			selection.setFillColor(sf::Color::Transparent);

			m_target.draw(selection);
		}
	}

private:
	sf::RenderTarget& m_target;
	Color m_selectionColor = 0xFF0000FF;
	float m_selectionThickness = 5;

	void ApplyTo(const Rectangle& rect) override
	{
		sf::RectangleShape drawable({ rect.GetWidth(), rect.GetHeight() });

		auto [x, y] = rect.GetTopLeft();
		drawable.setPosition({ x, y });
		drawable.setFillColor(sf::Color(rect.GetColor()));

		m_target.draw(drawable);
	}

	void ApplyTo(const Ellipse& ellipse) override
	{
		auto [x, y, w, h] = ellipse.GetBounds();

		sf::CircleShape drawable(w / 2);

		drawable.setPosition({ x, y });
		drawable.setScale({ 1, h / w });
		drawable.setFillColor(sf::Color(ellipse.GetColor()));

		m_target.draw(drawable);
	}

	void ApplyTo(const Triangle& triangle) override
	{
		sf::ConvexShape drawable;

		drawable.setPointCount(3);

		drawable.setPoint(0, ToSfVector(triangle.GetVertex1()));
		drawable.setPoint(1, ToSfVector(triangle.GetVertex2()));
		drawable.setPoint(2, ToSfVector(triangle.GetVertex3()));
		drawable.setFillColor(sf::Color(triangle.GetColor()));

		m_target.draw(drawable);
	}

	sf::Vector2f ToSfVector(Point point) const
	{
		return { point.x, point.y };
	}
};