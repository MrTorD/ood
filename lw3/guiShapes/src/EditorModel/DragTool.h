#pragma once

#include "ICommand.h"
#include "Point.h"
#include <memory>

// TODO: Избавиться от драгтула?
class DragTool
{
public:
	void OnMousePressed(Point point)
	{
		m_isDragging = true;
		m_startPos = point;
	}

	void OnMouseMoved(Point point)
	{
		if (m_isDragging)
		{
			UpdatePreview(point - m_startPos);
		}
	}

	void OnMouseReleased(Point point)
	{
		if (!m_isDragging)
		{
			return;
		}

		UpdatePreview(point - m_startPos);
		auto cmd = CreateCommand();
		ClearPreview();
		m_isDragging = false;

		if (cmd)
		{
			ExecuteCommand(std::move(cmd));
		}
	}

private:
	bool m_isDragging = false;
	Point m_startPos;

	virtual void UpdatePreview(Point point) = 0;
	virtual std::unique_ptr<ICommand> CreateCommand() = 0;
	virtual void ClearPreview() = 0;
    // TODO: think 'bout execute command methode
	virtual void ExecuteCommand(std::unique_ptr<ICommand> command) = 0;
};