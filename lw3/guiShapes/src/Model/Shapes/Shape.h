#pragma once

#include "Bounds.h"
#include "Color.h"
#include "IShapeVisitor.h"
#include <memory>

// [x]: Разобраться с четкостью Dran'n'drop
// [x]: Ограничить рамку
// [x]: Контекстные окна для загрузки документа
// [ ]: Диаграмма классов
class Shape
{
public:
	virtual Bounds GetBounds() const = 0;
	virtual void SetBounds(Bounds bounds) = 0;
	virtual void ShiftBy(Bounds offset) = 0;

	virtual bool IsContains(Point point) const = 0;
	virtual void Accept(IShapeVisitor& visitor) const = 0;
	virtual std::unique_ptr<Shape> Clone() const = 0;

	Color GetColor() const
	{
		return m_color;
	}

	void SetColor(Color color)
	{
		m_color = color;
	}

	virtual ~Shape() = default;

protected:
	Color m_color;
};