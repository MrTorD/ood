#pragma once

#include "AbstractCommand.h"
#include "../ParsingUtils.h"

class DeleteShapeCommand : public AbstractCommand
{
public:
	DeleteShapeCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
	}

	void Perform(Picture& picture) override
	{
		picture.DeleteShape(m_id);
	}

private:
	std::string m_id;
};