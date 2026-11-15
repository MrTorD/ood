#pragma once

#include "IEditorShapeVisitor.h"
#include "SelectionBox.h"
#include "Shape.h"
#include "TimelineBox.h"
#include "VisitorAdapter.h"

class EditorModel;

class EditorShape
{
public:
	Bounds GetBounds() const
	{
		auto bounds = m_shape.GetBounds();

		return bounds += m_previewOffset;
	}

	SelectionBox GetSelectionBox() const
	{
		return SelectionBox(GetBounds());
	};

	TimelineBox GetTimelineBox(size_t index) const
	{
		return TimelineBox(index, m_shape.GetTimeline());
	}

	Color GetColor() const
	{
		return m_shape.GetColor();
	}

	bool IsSelected() const
	{
		return m_isSelected;
	}

	bool SetSelected(bool isSelected)
	{
		std::swap(m_isSelected, isSelected);

		return m_isSelected != isSelected;
	}

	void Accept(IEditorShapeVisitor& visitor) const
	{
		VisitorAdapter adapter(*this, visitor);
		m_shape.Accept(adapter);
	}

	bool IsContains(Point point) const
	{
		return m_shape.IsContains({ point.x - m_previewOffset.x,
			point.y - m_previewOffset.y });
	}

	Bounds GetTimeline() const
	{
		auto [t, dt] = m_shape.GetTimeline();

		return { t + m_timelineOffset.x, m_timelineOffset.y, dt + m_timelineOffset.w, 0 };
	}

	void ResetPreview()
	{
		SetPreviewOffset({});
	}

private:
	IShape& m_shape;
	Bounds m_previewOffset;
	Bounds m_timelineOffset;

	bool m_isSelected = false;

	friend class EditorModel;

	explicit EditorShape(IShape& shape)
		: m_shape(shape)
	{
	}

	IShape& GetDomainShape()
	{
		return m_shape;
	}

	const IShape& GetDomainShape() const
	{
		return m_shape;
	}

	Bounds GetPreviewOffset() const
	{
		return m_previewOffset;
	}

	Bounds GetTimelineOffset() const
	{
		return m_timelineOffset;
	}

	void SetPreviewOffset(Bounds offset)
	{
		m_previewOffset = offset;
	}

	void SetTimelineOffset(Bounds offset)
	{
		m_timelineOffset = offset;
	}

	void ResetTimelineShift()
	{
		m_timelineOffset = Bounds();
	}
};