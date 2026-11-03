#pragma once

#include "../EditorModel/EditorShape/SelectionBox.h"
#include "PictureRenderer.h"
#include <algorithm>

class CanvasController
{
public:
	explicit CanvasController(EditorModel& model)
		: m_model(model)
	{
	}

	void OnMouseWithCtrlPressed(Point point)
	{
		auto* shape = m_model.FindShapeAt(point);

		if (shape)
		{
			shape->SetSelected(!shape->IsSelected());
		}
	}

	void OnMousePressed(Point point)
	{
		if (!m_model.GetBounds().IsContains(point))
		{
			return;
		}

		m_model.GetCanvasMode() == CanvasMode::SelectShapes
			? BeginShiftShape(point)
			: BeginCreateShape(point);
	}

	void OnMouseMoved(Point point)
	{
		if (!m_isInAction)
		{
			return;
		}

		m_model.UpdateActiveShapes(point - m_startPosition);
	}

	void OnMouseReleased(Point point)
	{
		if (!m_isInAction)
		{
			return;
		}

		m_model.UpdateActiveShapes(point - m_startPosition);

		m_model.CommitActiveShapes();

		m_isInAction = false;
	}

private:
	EditorModel& m_model;

	bool m_isInAction = false;
	Point m_startPosition;

	std::unique_ptr<Shape> m_tempShape;

	void BeginCreateShape(Point point)
	{
		auto [x, y, w, h] = m_model.GetBounds();
		auto [minShapeW, minShapeH] = m_model.GetMinimalShapeSize();

		if (Bounds availableBounds = { x, y, w - minShapeW, h - minShapeH }; !availableBounds.IsContains(point))
		{
			return;
		}

		CreateTempShape(point);
		m_model.BeginCreateShape(*m_tempShape.get());

		m_isInAction = true;
		m_startPosition = point;
	}

	void BeginShiftShape(Point point)
	{
		auto* shape = m_model.FindShapeAt(point);

		if (!shape)
		{
			m_model.UnselectAll();
			m_isInAction = false;

			return;
		}

		if (!shape->IsSelected())
		{
			m_model.UnselectAll();
			shape->SetSelected(true);
		}

		m_model.BeginShiftShapes(point);

		m_isInAction = true;
		m_startPosition = point;
	}

	using Rectangle = class Rectangle;
	using Ellipse = class Ellipse;

	void CreateTempShape(Point point)
	{
		switch (m_model.GetCanvasMode())
		{
		case CanvasMode::CreatingRectangle:
			m_tempShape = std::make_unique<Rectangle>(point, 1, 1, 0xFFC000FF);
			return;
		case CanvasMode::CreatingEllipse:
			m_tempShape = std::make_unique<Ellipse>(point, 1, 1, 0x4472C4FF);
			return;
		case CanvasMode::CreatingTriangle:
			m_tempShape = std::make_unique<Triangle>(point, 1, 1, 0x70AD47FF);
		}
	}
};