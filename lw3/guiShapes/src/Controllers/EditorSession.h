#pragma once

#include "SelectionBox.h"
#include "Shape.h"
#include <optional>

class EditorSession
{
public:
	const Shape* GetSelectedShape() const
	{
		return m_selectedShape;
	}

	Shape* GetSelectedShape()
	{
		return m_selectedShape;
	}

	void SetSelectedShape(Shape* shape)
	{
		m_selectedShape = shape;

		if (shape)
		{
			m_selectionBox = SelectionBox(shape->GetBounds());
		}
		else
		{
			m_selectionBox = std::nullopt;
		}
	}

	const std::optional<SelectionBox>& GetSelectionBox() const
	{
		return m_selectionBox;
	}

private:
	Shape* m_selectedShape = nullptr;
	std::optional<SelectionBox> m_selectionBox = std::nullopt;
};