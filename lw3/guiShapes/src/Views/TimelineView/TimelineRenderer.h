#pragma once

#include "EditorModel.h"
#include "IEditorShapeVisitor.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class TimelineRenderer final : public IEditorShapeVisitor
{
public:
	TimelineRenderer(sf::RenderTarget& target)
		: m_target(target)
	{
	}

	void Draw(const EditorModel& model)
	{
		for (int i = model.GetShapesCount() - 1; i >= 0; i--)
		{
			auto& shape = model.GetShape(i);
			shape.Accept(*this);
			DrawShapeBox(shape);
		}

		m_drawnShapesCount = 0;
	}

private:
	sf::RenderTarget& m_target;

	int m_drawnShapesCount = 0;
	// float m_shapePreviewWidth = 40;
	float m_dragPointRadius = 5;

	void VisitRectangle(const EditorShape& rect) override
	{
		auto [t, dy, dt, _] = rect.GetTimeline();

		sf::RectangleShape drawable({ dt, TimelineBox::GetHeight() });

		drawable.setPosition({ t, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() });
		drawable.setFillColor(sf::Color(rect.GetColor()));

		m_target.draw(drawable);
	}

	void VisitTriangle(const EditorShape& triangle) override
	{
		auto [t, dy, dt, _] = triangle.GetTimeline();

		sf::ConvexShape drawable;

		drawable.setPointCount(3);

		drawable.setPoint(0, { t, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() + TimelineBox::GetHeight() });
		drawable.setPoint(1, { t + dt / 2, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() });
		drawable.setPoint(2, { t + dt, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() + TimelineBox::GetHeight() });

		drawable.setFillColor(sf::Color(triangle.GetColor()));

		m_target.draw(drawable);
	}

	void VisitEllipse(const EditorShape& ellipse) override
	{
		auto [t, dy, dt, _] = ellipse.GetTimeline();

		sf::CircleShape drawable(dt / 2);

		drawable.setPosition({ t, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() });
		drawable.setScale({ 1, TimelineBox::GetHeight() / dt });
		drawable.setFillColor(sf::Color(ellipse.GetColor()));

		m_target.draw(drawable);
	}

	void DrawShapeBox(const EditorShape& shape)
	{
		auto [t, dy, dt, _] = shape.GetTimeline();

		sf::RectangleShape drawable({ dt, TimelineBox::GetHeight() });

		float y = m_drawnShapesCount++ * TimelineBox::GetTotalHeight() + dy;
		drawable.setPosition({ t, y });
		drawable.setOutlineThickness(-3);

		drawable.setOutlineColor(shape.IsSelected() ? sf::Color::Red : sf::Color::White);
		drawable.setFillColor(sf::Color::Transparent);

		m_target.draw(drawable);
		for (auto boundPoints = GetBoundPoints(t, y, dt, TimelineBox::GetHeight());
			const auto& point : boundPoints)
		{
			m_target.draw(point);
		}
	}

	std::vector<sf::CircleShape> GetBoundPoints(float x, float y, float w, float h) const
	{
		std::vector<sf::CircleShape> boundPoints;

		for (int i = 0; i < 2; i++)
		{
			boundPoints.emplace_back(m_dragPointRadius);
			boundPoints[i].setFillColor(sf::Color::White);
		}

		auto pointD = 2 * m_dragPointRadius;

		boundPoints[0].setPosition({ x, y + ((h - pointD) / 2) });
		boundPoints[1].setPosition({ x + w - pointD, y + ((h - pointD) / 2) });

		return boundPoints;
	}
};