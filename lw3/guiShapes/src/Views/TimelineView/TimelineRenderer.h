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
		, m_texture("../../../lw3/guiShapes/assets/group.png")
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
	sf::Texture m_texture;

	int m_drawnShapesCount = 0;
	static constexpr float m_shapePreviewWidth = 40;

	void VisitRectangle(const EditorShape& rect) override
	{
		auto [t, dy, dt, _] = rect.GetTimeline();

		sf::RectangleShape drawable({ m_shapePreviewWidth, TimelineBox::GetHeight() });

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
		drawable.setPoint(1, { t + m_shapePreviewWidth / 2, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() });
		drawable.setPoint(2, { t + m_shapePreviewWidth, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() + TimelineBox::GetHeight() });

		drawable.setFillColor(sf::Color(triangle.GetColor()));

		m_target.draw(drawable);
	}

	void VisitEllipse(const EditorShape& ellipse) override
	{
		auto [t, dy, dt, _] = ellipse.GetTimeline();

		sf::CircleShape drawable(m_shapePreviewWidth / 2);

		drawable.setPosition({ t, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() });
		drawable.setScale({ 1, TimelineBox::GetHeight() / m_shapePreviewWidth });
		drawable.setFillColor(sf::Color(ellipse.GetColor()));

		m_target.draw(drawable);
	}

	void VisitGroupShape(const EditorShape& shape, const GroupShape& group) override
	{
		auto [t, dy, dt, _] = shape.GetTimeline();

		sf::Sprite drawable(m_texture);
		drawable.setPosition({ t, dy + m_drawnShapesCount * TimelineBox::GetTotalHeight() });
		auto [w, h] = drawable.getLocalBounds().size;
		drawable.setScale({ m_shapePreviewWidth / w, TimelineBox::GetHeight() / h });

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

	static std::vector<sf::CircleShape> GetBoundPoints(float x, float y, float w, float h)
	{
		std::vector<sf::CircleShape> boundPoints;

		for (int i = 0; i < 2; i++)
		{
			boundPoints.emplace_back(TimelineBox::GetDragPointRadius());
			boundPoints[i].setFillColor(sf::Color(0x852EF8FF));
		}

		auto pointD = 2 * TimelineBox::GetDragPointRadius();

		boundPoints[0].setPosition({ x, y + (h - pointD) / 2 });
		boundPoints[1].setPosition({ x + w - pointD, y + (h - pointD) / 2 });

		return boundPoints;
	}
};