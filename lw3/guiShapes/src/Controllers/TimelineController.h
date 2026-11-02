#pragma once

#include "EditorModel.h"

class TimelineController
{
public:
	TimelineController(EditorModel& model, float shapeBoxWidth, float shapeBoxHeight, float shapeBoxPadding)
		: m_model(model)
		, m_shapeBoxWidth(shapeBoxWidth)
		, m_shapeBoxHeight(shapeBoxHeight)
		, m_shapeBoxPadding(shapeBoxPadding)
	{
	}

	void OnMouseWithCtrlPressed(Point point)
	{
		int index = point.y / (m_shapeBoxHeight + m_shapeBoxPadding) - 1;
		if (index >= m_model.GetShapesCount())
		{
			return;
		}

		auto& shape = m_model.GetShape(m_model.GetShapesCount() - 1 - index);
		shape.SetSelected(!shape.IsSelected());
	}

	void OnMousePressed(Point point)
	{
		int index = point.y / (m_shapeBoxHeight + m_shapeBoxPadding) - 1;
		if (index >= m_model.GetShapesCount())
		{
			return;
		}

		auto& shape = m_model.GetShape(m_model.GetShapesCount() - 1 - index);

		if (!shape.IsSelected())
		{
			m_model.UnselectAll();
			shape.SetSelected(true);
		}

		m_model.BeginTimelineShift();
		m_isInAction = true;
		m_startPosition = point;
	}

	void OnMouseMoved(Point point)
	{
		if (!m_isInAction)
		{
			return;
		}

		m_model.UpdateTimelineShift(point - m_startPosition);
	}

	void OnMouseReleased(Point point)
	{
		if (!m_isInAction)
		{
			return;
		}

		m_model.UpdateTimelineShift(point - m_startPosition);
		// TODO: Разобраться с магическим числом
		m_model.CommitTimelineShift({ 0, 65 });

		m_isInAction = false;
	}

private:
	EditorModel& m_model;
	float m_shapeBoxWidth;
	float m_shapeBoxHeight;
	float m_shapeBoxPadding;

	bool m_isInAction = false;
	Point m_startPosition;
};