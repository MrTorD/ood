#pragma once

#include "CommandHistory.h"
#include "Picture.h"

class PictureDocument
{
public:
	PictureDocument()
	{
		m_picture = std::make_unique<Picture>();
	}

	PictureDocument(std::unique_ptr<Picture> picture)
	{
		m_picture = std::move(picture);
	}

	Picture* GetPicture()
	{
		return m_picture.get();
	}

	// TODO: Подумать над тем, чтобы избавиться от метода
	void SetPicture(std::unique_ptr<Picture> picture)
	{
		m_picture = std::move(picture);
		// TODO: Избавиться от трункейта
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
	std::unique_ptr<Picture> m_picture;
	CommandHistory m_history;
};