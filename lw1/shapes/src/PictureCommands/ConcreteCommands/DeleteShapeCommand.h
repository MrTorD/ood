#pragma once

#include "ICommand.h"
#include "ParsingUtils.h"

class DeleteShapeCommand : public ICommand
{
public:
	DeleteShapeCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
	}

	void Execute(Picture& picture) override
	{
		picture.DeleteShape(m_id);
	}

private:
	std::string m_id;
};