#pragma once

#include "ICommand.h"
#include "../ParsingUtils.h"

class MovePictureCommand : public ICommand
{
public:
	MovePictureCommand(std::stringstream& ss)
	{
		m_dx = ReadDouble(ss);
		m_dy = ReadDouble(ss);
	}

	void Execute(Picture& picture) override
	{
		picture.MovePicture(m_dx, m_dy);
	}

private:
	double m_dx;
	double m_dy;
};