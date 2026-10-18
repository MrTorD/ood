#pragma once

#include "CommandHistory.h"
#include "Picture.h"

class PictureDocument
{
public:
	Picture& GetPicture()
	{
		return m_picture;
	}

	const Picture& GetPicture() const
	{
		return m_picture;
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
	Picture m_picture;
	CommandHistory m_history;
};