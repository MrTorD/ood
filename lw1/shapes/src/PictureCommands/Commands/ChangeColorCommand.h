#pragma once

#include "AbstractCommand.h"
#include "../ParsingUtils.h"

class ChangeColorCommand : public AbstractCommand
{
public:
	ChangeColorCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
		m_newColor = ReadColor(ss);
	}

	void Perform(Picture& picture) override
	{
		picture.ChangeShapeColor(m_id, m_newColor);
	}

private:
	std::string m_id;
	Color m_newColor;
};