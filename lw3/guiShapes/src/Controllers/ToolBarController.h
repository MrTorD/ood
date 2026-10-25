#pragma once

#include "Button.h"
#include "Ellipse.h"
#include "FileDialogues.h"
#include "Picture.h"
#include "PictureSerialiser.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <vector>

class ToolBarController
{
public:
	ToolBarController(EditorModel& model)
		: m_model(model)
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
		auto path = pfd::save_file("Select file").result();
		m_model.SaveToFile(path);
	}

	void LoadFromFile()
	{
		auto path = pfd::open_file("Select a file").result();
		m_model.LoadFromFile(path[0]);
	}

private:
	EditorModel& m_model;
};