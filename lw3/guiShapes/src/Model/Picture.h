#pragma once

#include "Shape.h"
#include <boost/signals2/signal.hpp>
#include <list>
#include <memory>
#include <ranges>

// [x]: Узнать про аналоги mvc
// [ ]: поправить диаграмму
// [ ]: узнать почему не работает драг в фулл экр
// [ ]: удаление клавшией delete
// [ ]: Поправить ux. вынести кнопки в контейнер, сделать обводку для границ
// [ ]:  Поправить границу маркера перетягивания
// [ ]: Обработать ситуацию, когда мышка была отпущена в альтабе
class Picture
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;
	using ChangeSignal = boost::signals2::signal<void()>;

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

	Shape* GetShape(size_t i)
	{
		if (i >= m_shapes.size())
		{
			throw std::out_of_range("Index is out of shape list bounds");
		}

		auto it = m_shapes.begin();
		std::advance(it, i);

		return it->get();
	}

	size_t GetIndex(const Shape* searchShape)
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

	Shape* FindShapeAt(Point point)
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

	void AddShape(std::unique_ptr<Shape> shape)
	{
		m_shapes.push_back(std::move(shape));
		m_changeSignal();
	}

	void AddShape(std::unique_ptr<Shape> shape, size_t index)
	{
		auto it = m_shapes.begin();
		std::advance(it, index);

		m_shapes.insert(it, std::move(shape));
		m_changeSignal();
	}

	void DeleteShape(size_t index)
	{
		auto it = m_shapes.begin();
		std::advance(it, index);

		m_shapes.erase(it);
		m_changeSignal();
	}

	[[nodiscard]] ScopedConnection DoOnChange(ChangeSignal::slot_type slot)
	{
		return m_changeSignal.connect(std::move(slot));
	}

private:
	std::list<std::unique_ptr<Shape>> m_shapes;
	ChangeSignal m_changeSignal;
};