#pragma once

#include "ICommand.h"
#include "ParsingUtils.h"

class CloneShapeCommand : public ICommand
{
public:
	CloneShapeCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
		m_newId = ReadStringStrictly(ss);
	}

	void Execute(Picture& picture) override
	{
		picture.CloneShape(m_id, m_newId);
	}

private:
	std::string m_id;
    std::string m_newId;
};