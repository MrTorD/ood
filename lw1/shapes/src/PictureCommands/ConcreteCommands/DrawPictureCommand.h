#pragma once

#include "ICommand.h"
#include "ParsingUtils.h"

class DrawPictureCommand : public ICommand
{
public:
	DrawPictureCommand(std::stringstream& ss)
	{
	}

	void Execute(Picture& picture) override
	{
		picture.DrawPicture();
	}
};