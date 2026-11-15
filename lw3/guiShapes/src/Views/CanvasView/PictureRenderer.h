#pragma once

#include "EditorModel.h"
#include "IEditorShapeVisitor.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class PictureRenderer final : public IEditorShapeVisitor
	, private IShapeVisitor
{
public:
	// [x]: Избавиться от зависимости View oт Model
	explicit PictureRenderer(sf::RenderTarget& target)
		: m_target(target)
	{
	}

	void Draw(const EditorModel& model)
	{
		for (int i = 0; i < model.GetShapesCount(); i++)
		{
			if (auto& shape = model.GetShape(i); model.IsShapeTimeBetween(&shape))
			{
				shape.Accept(*this);
			}
		}

		for (auto shape : model.GetSelectedShapes())
		{
			if (model.IsShapeTimeBetween(shape))
			{
				DrawSelection(*shape);
			}
		}
	}

private:
	sf::RenderTarget& m_target;
	Color m_selectionColor = 0xFF0000FF;
	float m_selectionThickness = -7;
	float m_dragPointRadius = 5;
	Bounds m_groupOffset;

	void VisitRectangle(const EditorShape& rect) override
	{
		DrawRectangle(rect.GetBounds(), rect.GetColor());
	}

	void VisitEllipse(const EditorShape& ellipse) override
	{
		DrawEllipse(ellipse.GetBounds(), ellipse.GetColor());
	}

	void VisitTriangle(const EditorShape& triangle) override
	{
		DrawTriangle(triangle.GetBounds(), triangle.GetColor());
	}

	void VisitGroupShape(const EditorShape& shape, const GroupShape& group) override
	{
		m_groupOffset = shape.GetBounds() - group.GetBounds();

		group.Accept(*this);
		m_groupOffset = {};
	}

	void Visit(const Rectangle& rect) override
	{
		DrawRectangle(rect.GetBounds() + m_groupOffset, rect.GetColor());
	}

	void Visit(const Ellipse& ellipse) override
	{
		DrawEllipse(ellipse.GetBounds() + m_groupOffset, ellipse.GetColor());
	}

	void Visit(const Triangle& triangle) override
	{
		DrawTriangle(triangle.GetBounds() + m_groupOffset, triangle.GetColor());
	}

	void Visit(const GroupShape& group) override
	{
		for (size_t i = 0; i < group.GetShapeCount(); i++)
		{
			group.GetShape(i).Accept(*this);
		}
	}

	void DrawEllipse(Bounds bounds, Color color)
	{
		auto [x, y, w, h] = bounds;

		sf::CircleShape drawable(w / 2);

		drawable.setPosition({ x, y });
		drawable.setScale({ 1, h / w });
		drawable.setFillColor(sf::Color(color));

		m_target.draw(drawable);
	}

	void DrawTriangle(Bounds bounds, Color color)
	{
		auto [x, y, w, h] = bounds;

		sf::ConvexShape drawable;

		drawable.setPointCount(3);

		drawable.setPoint(0, { x, y + h });
		drawable.setPoint(1, { x + (w / 2), y });
		drawable.setPoint(2, { x + w, y + h });

		drawable.setFillColor(sf::Color(color));

		m_target.draw(drawable);
	}

	void DrawRectangle(Bounds bounds, Color color)
	{
		auto [x, y, w, h] = bounds;

		sf::RectangleShape drawable({ w, h });

		drawable.setPosition({ x, y });
		drawable.setFillColor(sf::Color(color));

		m_target.draw(drawable);
	}

	void DrawSelection(const EditorShape& shape) const
	{
		auto [x, y, w, h] = shape.GetBounds();
		sf::RectangleShape selection({ w, h });

		selection.setPosition({ x, y });
		selection.setOutlineColor(sf::Color(m_selectionColor));
		selection.setOutlineThickness(m_selectionThickness);
		selection.setFillColor(sf::Color::Transparent);

		m_target.draw(selection);

		for (auto boundPoints = GetBoundPoints(x, y, w, h);
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
};