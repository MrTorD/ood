#pragma once

#include "CommandHistory.h"
#include "Picture.h"

class PictureDocument
{
public:
	Picture* GetPicture()
	{
		return m_picture.get();
	}

	void SetPicture(std::unique_ptr<Picture> picture)
	{
		m_picture = std::move(picture);
		m_history.Truncate();
	}

	void Undo()
	{
		m_history.Undo();
	}

	void Redo()
	{
		m_history.Redo();
	}

	void ExecuteCommand(std::unique_ptr<ICommand> command)
	{
		m_history.Execute(std::move(command));
	}

private:
	std::unique_ptr<Picture> m_picture = std::make_unique<Picture>();
	CommandHistory m_history;
};