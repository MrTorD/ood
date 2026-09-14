#pragma once

#include "AbstractCommand.h"
#include "../ParsingUtils.h"

class DrawShapeCommand : public AbstractCommand
{
public:
	DrawShapeCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
	}

	void Perform(Picture& picture) override
	{
		picture.DrawShape(m_id);
	}

private:
	std::string m_id;
};