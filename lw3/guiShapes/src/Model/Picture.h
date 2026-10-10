#pragma once

#include "Shape.h"
#include <list>
#include <memory>
#include <ranges>

//TODO: Узнать про аналоги mvc
class Picture
{
public:
	const Shape& GetShape(size_t i) const
	{
		if (i >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of shape list bounds");
		}

		auto it = m_shapes.begin();
		std::advance(it, i);

		return *(it->get());
	}

	Shape* FindShapeAt(Point point)
	{
		for (const auto& shape : m_shapes | std::views::reverse)
		{
			if (shape->HitTest(point))
			{
				return shape.get();
			}
		}

		return nullptr;
	}

	size_t GetShapesCount() const
	{
		return m_shapes.size();
	}

	void AddShape(std::unique_ptr<Shape> shape)
	{
		m_shapes.push_back(std::move(shape));
	}

	void DeleteShape(Shape* removedShape)
	{
		m_shapes.remove_if([&removedShape](const std::unique_ptr<Shape>& shape) {
			return shape.get() == removedShape;
		});
	}

private:
	std::list<std::unique_ptr<Shape>> m_shapes;
};