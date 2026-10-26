#pragma once

#include "EditorModel.h"
#include "Ellipse.h"
#include "FileDialogues.h"
#include "Rectangle.h"
#include "Triangle.h"

class ToolBarController
{
public:
	ToolBarController(
		EditorModel& model,
		std::function<void()> createNewWindow,
		std::function<void(std::unique_ptr<Picture>)> reopenWindow,
		std::function<void()> makeWindowCopy)
		: m_model(model)
		, m_createNewWindow(std::move(createNewWindow))
		, m_reopenWindow(std::move(reopenWindow))
		, m_makeWindowCopy(std::move(makeWindowCopy))
	{
		// [x]: Контроллер не должен знать про расположение кнопок
	}

	using Rectangle = class Rectangle;
	using Ellipse = class Ellipse;

	void AddRectangle()
	{
		m_model.AddShape(std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF));
	}

	void AddEllipse()
	{
		m_model.AddShape(std::make_unique<Ellipse>(Point(320, 220), 60, 30, 0xFFDE21FF));
	}

	void AddTriangle()
	{
		m_model.AddShape(std::make_unique<Triangle>(Point(300, 250), 60, 60, 0xF9AAC0FF));
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

	void OpenFromFile()
	{
		auto path = pfd::open_file("Select a file", ".").result();
		if (!path.empty())
		{
			PictureSerialiser serialiser;
			auto picture = serialiser.Deserialise(path[0]);
			m_reopenWindow(std::move(picture));
		}
	}

	void CreateNewWindow()
	{
		m_createNewWindow();
	}

	void MakeWindowCopy()
	{
		m_makeWindowCopy();
	}

private:
	EditorModel& m_model;
	std::function<void()> m_createNewWindow;
	std::function<void(std::unique_ptr<Picture>)> m_reopenWindow;
	std::function<void()> m_makeWindowCopy;
};