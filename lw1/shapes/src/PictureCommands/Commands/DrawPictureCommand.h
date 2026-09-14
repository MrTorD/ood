#pragma once

#include "AbstractCommand.h"
#include "../ParsingUtils.h"

class DrawPictureCommand : public AbstractCommand
{
public:
	DrawPictureCommand(std::stringstream& ss)
	{
	}

	void Perform(Picture& picture) override
	{
		picture.DrawPicture();
	}
};