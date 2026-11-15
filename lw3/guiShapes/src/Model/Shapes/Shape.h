#pragma once

#include "IShape.h"

// [x]: Разобраться с четкостью Dran'n'drop
// [x]: Ограничить рамку
// [x]: Контекстные окна для загрузки документа
// [x]: Диаграмма классов
class Shape : public IShape
{
public:
	virtual void SetBounds(Bounds bounds) = 0;

	Point GetTimeline() const override
	{
		return m_timeline;
	}

	void SetTimeline(Point timeline) override
	{
		m_timeline = timeline;
	}

	Color GetColor() const override
	{
		return m_color;
	}

	void SetColor(Color color)
	{
		m_color = color;
	}

protected:
	Color m_color;
	Point m_timeline;
};