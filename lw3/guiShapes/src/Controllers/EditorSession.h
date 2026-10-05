#pragma once

#include "Shape.h"

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

	Bounds GetAddRectButtonBounds() const
	{
		return m_addRectBounds;
	}

	Bounds GetAddEllipseBounds() const
	{
		return m_addEllipseBounds;
	}

	Bounds GetAddTriangleBounds() const
	{
		return m_addTriangleBounds;
	}

	Bounds GetDeleteSelectedBounds() const
	{
		return m_deleteSelectedBounds;
	}

	void SetSelectedShape(Shape* shape)
	{
		m_selectedShape = shape;
	}

private:
	Shape* m_selectedShape = nullptr;
	Bounds m_addRectBounds = { 0, 0, 20, 20 };
	Bounds m_addEllipseBounds = { 0, 30, 20, 20 };
	Bounds m_addTriangleBounds = { 0, 60, 20, 20 };
	Bounds m_deleteSelectedBounds = { 0, 90, 20, 20 };
};