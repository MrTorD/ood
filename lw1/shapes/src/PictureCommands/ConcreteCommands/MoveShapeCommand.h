#pragma once

#include "ICommand.h"
#include "ParsingUtils.h"

class MoveShapeCommand : public ICommand
{
public:
	MoveShapeCommand(std::stringstream& ss)
	{
		m_id = ReadStringStrictly(ss);
		m_dx = ReadDouble(ss);
		m_dy = ReadDouble(ss);
	}

	void Execute(Picture& picture) override
	{
		picture.MoveShape(m_id, m_dx, m_dy);
	}

private:
	std::string m_id;
	double m_dx;
	double m_dy;
};