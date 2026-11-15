#pragma once

#include "IShape.h"
#include "ShapeCollection.h"
#include <boost/signals2/signal.hpp>
#include <list>
#include <memory>
#include <ranges>

// [x]: Узнать про аналоги MVC
// [x]: Поправить диаграмму классов
// [x]: Узнать почему не работает драг в полном экране
// [x]: Удаление выделенных фигур клавишей delete
// [x]: Поправить UX. Вынести кнопки в контейнер, сделать обводку для границ
// [x]: Поправить границу маркера перетягивания
// [x]: Обработать ситуацию, когда мышка была отпущена в альтабе
// [x]: Рисовать выделение поверх всего
class Picture
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;
	using ChangeSignal = boost::signals2::signal<void()>;

	const IShape& GetShape(size_t i) const
	{
		return m_shapes.GetShape(i);
	}

	IShape& GetShape(size_t i)
	{
		return m_shapes.GetShape(i);
	}

	size_t GetIndex(const IShape* searchShape) const
	{
		return m_shapes.GetIndex(searchShape);
	}

	IShape* FindShapeAt(Point point)
	{
		return m_shapes.FindShapeAt(point);
	}

	size_t GetShapesCount() const
	{
		return m_shapes.GetShapesCount();
	}

	void AddShape(std::unique_ptr<IShape> shape)
	{
		m_shapes.AddShape(std::move(shape));
		m_changeSignal();
	}

	void AddShape(std::unique_ptr<IShape> shape, size_t index)
	{
		m_shapes.AddShape(std::move(shape), index);
		m_changeSignal();
	}

	[[nodiscard]] std::unique_ptr<IShape> ExtractShape(size_t index)
	{
		return m_shapes.ExtractShape(index);
	}

	void LiftShape(size_t index, int liftLayersCount)
	{
		m_shapes.LiftShape(index, liftLayersCount);
		m_changeSignal();
	}

	void DeleteShape(size_t index)
	{
		m_shapes.DeleteShape(index);
		m_changeSignal();
	}

	[[nodiscard]] ScopedConnection DoOnChange(const ChangeSignal::slot_type& slot)
	{
		return m_changeSignal.connect(slot);
	}

private:
	ShapeCollection m_shapes;
	ChangeSignal m_changeSignal;
};