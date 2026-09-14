#pragma once

#include "AbstractCommand.h"
#include "../ParsingUtils.h"

class MovePictureCommand : public AbstractCommand
{
public:
	MovePictureCommand(std::stringstream& ss)
	{
		m_dx = ReadDouble(ss);
		m_dy = ReadDouble(ss);
	}

	void Perform(Picture& picture) override
	{
		picture.MovePicture(m_dx, m_dy);
	}

private:
	double m_dx;
	double m_dy;
};