#pragma once

#include "AbstractCommand.h"
#include "../ParsingUtils.h"

class ChangeShapeCommand : public AbstractCommand
{
public:
	ChangeShapeCommand(std::stringstream& ss)
	{
		m_data.id = ReadStringStrictly(ss);
		m_data.shapeType = ReadShapeType(ReadStringStrictly(ss));

		if (m_data.shapeType == ShapeType::Unknown)
		{
			throw CommandParseError("Invalid shape provided");
		}

		ReadCommandData(ss, m_data);
	}

	void Perform(Picture& picture) override
	{
		picture.ChangeShape(m_data.id, CreateGeometry(m_data), m_data.bounds);
	}

private:
	CommandData m_data;
};