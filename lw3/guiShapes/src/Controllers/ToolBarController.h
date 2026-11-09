#pragma once

#include "EditorModel.h"
#include "FileDialogues.h"

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

	void SetCanvasMode(CanvasMode mode)
	{
		m_model.SetCanvasMode(mode);
	}

	void DeleteSelectedShapes()
	{
		m_model.DeleteSelectedShapes();
	}

	void SaveToFile()
	{
		if (auto path = pfd::save_file("Select a file", ".").result(); !path.empty())
		{
			m_model.SaveToFile(path);
		}
	}

	void OpenFromFile() const
	{
		if (auto path = pfd::open_file("Select a file", ".").result(); !path.empty())
		{
			auto picture = PictureSerializer::Deserialize(path[0]);
			m_reopenWindow(std::move(picture));
		}
	}

	void CreateNewWindow() const
	{
		m_createNewWindow();
	}

	void MakeWindowCopy() const
	{
		m_makeWindowCopy();
	}

private:
	EditorModel& m_model;
	std::function<void()> m_createNewWindow;
	std::function<void(std::unique_ptr<Picture>)> m_reopenWindow;
	std::function<void()> m_makeWindowCopy;
};