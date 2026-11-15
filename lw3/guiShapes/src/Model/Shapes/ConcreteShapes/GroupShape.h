#pragma once

#include "IShape.h"
#include "ShapeCollection.h"
#include "Visitable.h"

class GroupShape : public Visitable<GroupShape, IShape>
{
public:
	size_t GetShapeCount() const noexcept
	{
		return m_children.GetShapesCount();
	}

	IShape& GetShape(size_t index)
	{
		return m_children.GetShape(index);
	}

	const IShape& GetShape(size_t index) const
	{
		return m_children.GetShape(index);
	}

	Bounds GetBounds() const override
	{
		if (m_children.GetShapesCount() == 0)
		{
			return {};
		}

		auto first = m_children.GetShape(0).GetBounds();
		float left = first.x;
		float top = first.y;
		float right = first.x + first.w;
		float bottom = first.y + first.h;
		for (size_t i = 1; i < m_children.GetShapesCount(); i++)
		{
			auto [x, y, w, h] = m_children.GetShape(i).GetBounds();
			left = std::min(left, x);
			top = std::min(top, y);
			right = std::max(right, x + w);
			bottom = std::max(bottom, y + h);
		}

		return { left, top, right - left, bottom - top };
	}

	Color GetColor() const override
	{
		return 0;
	}

	void ShiftBy(Bounds offset) override
	{
		for (size_t i = 0; i < m_children.GetShapesCount(); i++)
		{
			m_children.GetShape(i).ShiftBy(offset);
		}
	}

	Point GetTimeline() const override
	{
		float left = m_children.GetShape(0).GetTimeline().x;
		float right = left + m_children.GetShape(0).GetTimeline().y;

		for (size_t i = 1; i < m_children.GetShapesCount(); i++)
		{
			auto [t, dt] = m_children.GetShape(i).GetTimeline();
			left = std::min(left, t);
			right = std::max(right, left + dt);
		}

		return { left, right - left };
	}

	void SetTimeline(Point timeline) override
	{
	}

	void AddShape(std::unique_ptr<IShape> shape)
	{
		m_children.AddShape(std::move(shape));
	}

	void AddShape(std::unique_ptr<IShape> shape, size_t index)
	{
		m_children.AddShape(std::move(shape), index);
	}

	[[nodiscard]] std::unique_ptr<IShape> ExtractShape(size_t index)
	{
		return m_children.ExtractShape(index);
	}

	bool IsContains(Point point) const override
	{
		for (size_t i = 0; i < m_children.GetShapesCount(); i++)
		{
			if (m_children.GetShape(i).IsContains(point))
			{
				return true;
			}
		}

		return false;
	}

	std::unique_ptr<IShape> Clone() const override
	{
		auto clone = std::make_unique<GroupShape>();
		for (size_t i = 0; i < m_children.GetShapesCount(); i++)
		{
			clone->AddShape(m_children.GetShape(i).Clone());
		}

		return clone;
	}

private:
	ShapeCollection m_children;
};