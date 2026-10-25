#pragma once

#include "EditorModel.h"
#include "IEditorShapeVisitor.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class PictureRenderer : public IEditorShapeVisitor
{
public:
	// [x]: Избавиться от зависимости View oт Model
	PictureRenderer(sf::RenderTarget& target, Point offset)
		: m_target(target)
		, m_offset(offset)
	{
	}

	void Draw(const EditorModel& model)
	{
		for (int i = 0; i < model.GetShapesCount(); i++)
		{
			auto& shape = model.GetShape(i);
			shape.Accept(*this);
		}

		for (auto shape : model.GetSelectedShapes())
		{
			DrawSelection(*shape);
		}
	}

private:
	sf::RenderTarget& m_target;
	Point m_offset;
	Color m_selectionColor = 0xFF0000FF;
	float m_selectionThickness = -7;
	float m_dragPointRadius = 7;

	void VisitRectangle(const EditorShape& rect) override
	{
		auto [x, y, w, h] = rect.GetBounds();

		sf::RectangleShape drawable({ w, h });

		drawable.setPosition({ x + m_offset.x, y + m_offset.y });
		drawable.setFillColor(sf::Color(rect.GetColor()));

		m_target.draw(drawable);
	}

	void VisitEllipse(const EditorShape& ellipse) override
	{
		auto [x, y, w, h] = ellipse.GetBounds();

		sf::CircleShape drawable(w / 2);

		drawable.setPosition({ x + m_offset.x, y + m_offset.y });
		drawable.setScale({ 1, h / w });
		drawable.setFillColor(sf::Color(ellipse.GetColor()));

		m_target.draw(drawable);
	}

	void VisitTriangle(const EditorShape& triangle) override
	{
		auto [x, y, w, h] = triangle.GetBounds();

		sf::ConvexShape drawable;

		drawable.setPointCount(3);

		drawable.setPoint(0, { x + m_offset.x, y + h + m_offset.y });
		drawable.setPoint(1, { x + (w / 2) + m_offset.x, y + m_offset.y });
		drawable.setPoint(2, { x + w + m_offset.x, y + h + m_offset.y });

		drawable.setFillColor(sf::Color(triangle.GetColor()));

		m_target.draw(drawable);
	}

	void DrawSelection(const EditorShape& shape) const
	{
		auto [x, y, w, h] = shape.GetBounds();
		sf::RectangleShape selection({ w, h });

		selection.setPosition({ x + m_offset.x, y + m_offset.y });
		selection.setOutlineColor(sf::Color(m_selectionColor));
		selection.setOutlineThickness(m_selectionThickness);
		selection.setFillColor(sf::Color::Transparent);

		m_target.draw(selection);

		for (auto boundPoints = GetBoundPoints(x + m_offset.x, y + m_offset.y, w, h);
			const auto& point : boundPoints)
		{
			m_target.draw(point);
		}
	}

	std::vector<sf::CircleShape> GetBoundPoints(float x, float y, float w, float h) const
	{
		std::vector<sf::CircleShape> boundPoints;

		for (int i = 0; i < 8; i++)
		{
			boundPoints.emplace_back(m_dragPointRadius);
			boundPoints[i].setFillColor(sf::Color::White);
		}

		auto pointD = 2 * m_dragPointRadius;

		boundPoints[0].setPosition({ x, y });
		boundPoints[1].setPosition({ x + ((w - pointD) / 2), y });
		boundPoints[2].setPosition({ x + w - pointD, y });
		boundPoints[3].setPosition({ x + w - pointD, y + ((h - pointD) / 2) });
		boundPoints[4].setPosition({ x + w - pointD, y + h - pointD });
		boundPoints[5].setPosition({ x + ((w - pointD) / 2), y + h - pointD });
		boundPoints[6].setPosition({ x, y + h - pointD });
		boundPoints[7].setPosition({ x, y + ((h - pointD) / 2) });

		return boundPoints;
	}

	sf::Vector2f ToSfVector(Point point) const
	{
		return { point.x, point.y };
	}
};