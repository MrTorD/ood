#pragma once

#include "ICommand.h"
#include "../ParsingUtils.h"

class DrawShapeCommand : public ICommand
{
public:
	DrawShapeCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
	}

	void Execute(Picture& picture) override
	{
		picture.DrawShape(m_id);
	}

private:
	std::string m_id;
};