#pragma once

#include "EditorModel.h"

class TimelineController
{
public:
	TimelineController(EditorModel& model)
		: m_model(model)
	{
	}

	void OnMouseWithCtrlPressed(Point point)
	{
		if (point.x < 0 || point.y < 0) // TODO: Подумать обязателен ли он тут?
		{
			return;
		}

		int index = point.y / TimelineBox::GetTotalHeight();
		if (index >= m_model.GetShapesCount())
		{
			return;
		}

		auto& shape = m_model.GetShape(m_model.GetShapesCount() - 1 - index);
		if (shape.GetTimelineBox(index).IsContains(point))
		{
			shape.SetSelected(!shape.IsSelected());
		}
	}

	void OnMousePressed(Point point)
	{
		if (point.x < 0 || point.y < 0) // TODO: Подумать обязателен ли он тут?
		{
			return;
		}

		int index = point.y / TimelineBox::GetTotalHeight();
		if (index >= m_model.GetShapesCount())
		{
			m_model.UnselectAll();
			m_model.SetCurrentTime(point.x);
			return;
		}

		auto& shape = m_model.GetShape(m_model.GetShapesCount() - 1 - index);
		auto timelineBox = shape.GetTimelineBox(index);

		if (!timelineBox.IsContains(point))
		{
			m_model.UnselectAll();
			m_model.SetCurrentTime(point.x);
			return;
		}

		if (!shape.IsSelected())
		{
			m_model.UnselectAll();
			shape.SetSelected(true);
		}

		m_model.BeginTimelineShift(point);
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
		m_model.CommitTimelineShift();

		m_isInAction = false;
	}

private:
	EditorModel& m_model;

	bool m_isInAction = false;
	Point m_startPosition;
};