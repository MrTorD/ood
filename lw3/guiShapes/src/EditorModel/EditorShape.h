#pragma once

#include "IEditorShapeVisitor.h"
#include "SelectionBox.h"
#include "Shape.h"
#include "VisitorAdapter.h"

class EditorModel;

class EditorShape
{
public:
	Bounds GetBounds() const
	{
		auto bounds = m_shape.GetBounds();

		bounds += m_previewOffset;

		return bounds;
	}

	SelectionBox GetSelectionBox() const
	{
		return SelectionBox(GetBounds());
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

	bool HitTest(Point point) const
	{
		return m_shape.HitTest({ point.x - m_previewOffset.x,
			point.y - m_previewOffset.y });
	}

	bool ResetPreview()
	{
		return SetPreviewOffset({});
	}

private:
	Shape& m_shape;
	Bounds m_previewOffset;

	bool m_isSelected = false;

	friend class EditorModel;

	explicit EditorShape(Shape& shape)
		: m_shape(shape)
	{
	}

	Shape& GetDomainShape()
	{
		return m_shape;
	}

	const Shape& GetDomainShape() const
	{
		return m_shape;
	}

	Bounds GetPreviewOffset() const
	{
		return m_previewOffset;
	}

	bool SetPreviewOffset(Bounds offset)
	{
		std::swap(m_previewOffset, offset);

		return m_previewOffset != offset;
	}
};