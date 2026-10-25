#pragma once

#include "EditorModel.h"
#include "FileDialogues.h"

class ToolBarController
{
public:
	ToolBarController(EditorModel& model, std::function<void(std::unique_ptr<Picture>)> addWindowAction )
		: m_model(model)
		, m_addWindowAction(addWindowAction)
	{
		// [x]: Контроллер не должен знать про расположение кнопок
	}

	void AddRectangle()
	{
		// m_model.AddShape(std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF));
	}

	void AddEllipse()
	{
		// m_model.AddShape(std::make_unique<Ellipse>(Point(320, 220), 60, 30, 0xFFDE21FF));
	}

	void AddTriangle()
	{
		// m_model.AddShape(std::make_unique<Triangle>(Point(300, 250), 60, 60, 0xF9AAC0FF));
	}

	void DeleteSelectedShapes()
	{
		m_model.DeleteSelectedShapes();
	}

	void SaveToFile()
	{
		auto path = pfd::save_file("Select a file", ".").result();
		if (!path.empty())
		{
			m_model.SaveToFile(path);
		}
	}

	void LoadFromFile()
	{
		auto path = pfd::open_file("Select a file", ".").result();
		if (!path.empty())
		{
			auto picture = m_model.LoadPictureFromFile(path[0]);
			m_addWindowAction(std::move(picture));
		}
	}

private:
	EditorModel& m_model;
	std::function<void(std::unique_ptr<Picture>)> m_addWindowAction;
};