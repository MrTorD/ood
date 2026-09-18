#pragma once

#include "ICommand.h"
#include "ParsingUtils.h"

class ChangeColorCommand : public ICommand
{
public:
	ChangeColorCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
		m_newColor = ReadColor(ss);
	}

	void Execute(Picture& picture) override
	{
		picture.ChangeShapeColor(m_id, m_newColor);
	}

private:
	std::string m_id;
	Color m_newColor;
};