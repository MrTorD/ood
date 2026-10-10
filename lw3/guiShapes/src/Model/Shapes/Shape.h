#pragma once

#include "Bounds.h"
#include "Color.h"
#include "IShapeOperation.h"

// TODO: Разобраться с четкостью Dran'n'drop
// TODO: Ограничить рамку
// TODO: Контекстные окна для загрузки документа
// TODO: Диаграмма классов
class Shape
{
public:
	virtual Bounds GetBounds() const = 0;
	virtual void MoveBy(Point offset) = 0;

	virtual bool HitTest(Point point) const = 0;
	virtual void ApplyOperation(IShapeOperation& operation) const = 0;

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