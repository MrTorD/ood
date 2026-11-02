#pragma once

#include "EditorModel.h"
#include "IEditorShapeVisitor.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class TimelineRenderer : public IEditorShapeVisitor
{
public:
	TimelineRenderer(sf::RenderTarget& target, float shapeBoxWidth, float shapeBoxHeight, float shapeBoxPadding)
		: m_target(target)
		, m_shapeBoxWidth(shapeBoxWidth)
		, m_shapeBoxHeight(shapeBoxHeight)
		, m_shapeBoxPadding(shapeBoxPadding)
	{
	}

	void Draw(const EditorModel& model)
	{
		for (int i = model.GetShapesCount() - 1; i >= 0; i--)
		{
			auto& shape = model.GetShape(i);
			DrawShapeBox(shape);
			shape.Accept(*this);
		}
	}

private:
	sf::RenderTarget& m_target;

	int m_drawedShapesCount = 0;
	float m_shapePreviewWidth = 40;

	float m_shapeBoxWidth;
	float m_shapeBoxHeight;
	float m_shapeBoxPadding;

	void VisitRectangle(const EditorShape& rect) override
	{
		auto [x, y, w, h] = rect.GetTimelineOffset();
		
		sf::RectangleShape drawable({ m_shapePreviewWidth, m_shapeBoxHeight });

		drawable.setPosition({ x, y + m_drawedShapesCount * (m_shapeBoxHeight + m_shapeBoxPadding) });
		drawable.setFillColor(sf::Color(rect.GetColor()));

		m_target.draw(drawable);
	}

	void VisitTriangle(const EditorShape& triangle) override
	{
		auto [x, y, w, h] = triangle.GetTimelineOffset();

		sf::ConvexShape drawable;

		drawable.setPointCount(3);

		drawable.setPoint(0, { x, y + m_drawedShapesCount * (m_shapeBoxHeight + m_shapeBoxPadding) + m_shapeBoxHeight });
		drawable.setPoint(1, { x + m_shapePreviewWidth / 2, y + m_drawedShapesCount * (m_shapeBoxHeight + m_shapeBoxPadding) });
		drawable.setPoint(2, { x + m_shapePreviewWidth, y + m_drawedShapesCount * (m_shapeBoxHeight + m_shapeBoxPadding) + m_shapeBoxHeight });

		drawable.setFillColor(sf::Color(triangle.GetColor()));

		m_target.draw(drawable);
	}

	void VisitEllipse(const EditorShape& ellipse) override
	{
		auto [x, y, w, h] = ellipse.GetTimelineOffset();

		sf::CircleShape drawable(m_shapePreviewWidth / 2);

		drawable.setPosition({ x, m_drawedShapesCount * (m_shapeBoxHeight + m_shapeBoxPadding) + y });
		drawable.setScale({ 1, m_shapeBoxHeight / m_shapePreviewWidth });
		drawable.setFillColor(sf::Color(ellipse.GetColor()));

		m_target.draw(drawable);
	}

	void DrawShapeBox(const EditorShape& shape)
	{
		sf::RectangleShape drawable({ m_shapeBoxWidth, m_shapeBoxHeight });
		auto [x, y, w, h] = shape.GetTimelineOffset();

		drawable.setPosition({ x, ++m_drawedShapesCount * (m_shapeBoxHeight + m_shapeBoxPadding) + y });
		drawable.setOutlineThickness(3);

		drawable.setOutlineColor(shape.IsSelected() ? sf::Color::Red : sf::Color::White);
		drawable.setFillColor(sf::Color::Transparent);

		m_target.draw(drawable);
	}
};