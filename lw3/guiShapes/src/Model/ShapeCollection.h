#pragma once

#include "IShape.h"
#include <list>
#include <memory>
#include <ranges>

class ShapeCollection
{
public:
	const IShape& GetShape(size_t i) const
	{
		if (i >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of shape list bounds");
		}

		return *(*std::next(m_shapes.begin(), i));
	}

	IShape& GetShape(size_t i)
	{
		if (i >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of shape list bounds");
		}

		return *(*std::next(m_shapes.begin(), i));
	}

	size_t GetIndex(const IShape* searchShape) const
	{
		size_t index = 0;
		for (const auto& shape : m_shapes)
		{
			if (shape.get() == searchShape)
			{
				return index;
			}

			index++;
		}

		return -1;
	}

	IShape* FindShapeAt(Point point)
	{
		for (const auto& shape : m_shapes | std::views::reverse)
		{
			if (shape->IsContains(point))
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

	void AddShape(std::unique_ptr<IShape> shape)
	{
		m_shapes.push_back(std::move(shape));
	}

	void AddShape(std::unique_ptr<IShape> shape, size_t index)
	{
		auto it = std::next(m_shapes.begin(), index);

		m_shapes.insert(it, std::move(shape));
	}

	void LiftShape(size_t index, int liftLayersCount)
	{
		auto it = std::next(m_shapes.begin(), index);
		auto newPlaceIt = liftLayersCount > 0
			? std::next(it, liftLayersCount + 1)
			: std::prev(it, -liftLayersCount);

		m_shapes.splice(newPlaceIt, m_shapes, it);
	}

	[[nodiscard]] std::unique_ptr<IShape> ExtractShape(size_t index)
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of shape list bounds");
		}

		auto it = std::next(m_shapes.begin(), index);
		auto shape = std::move(*it);
		m_shapes.erase(it);

		return shape;
	}

	void DeleteShape(size_t index)
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of shape list bounds");
		}

		auto it = std::next(m_shapes.begin(), index);
		m_shapes.erase(it);
	}

private:
	std::list<std::unique_ptr<IShape>> m_shapes;
};