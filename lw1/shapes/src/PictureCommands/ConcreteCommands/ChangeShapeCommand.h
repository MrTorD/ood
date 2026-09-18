#pragma once

#include "ICommand.h"
#include "ParsingUtils.h"

class ChangeShapeCommand : public ICommand
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

		ReadConcreteShapeParams(ss, m_data);
	}

	void Execute(Picture& picture) override
	{
		picture.ChangeShape(m_data.id, CreateGeometry(m_data));
	}

private:
	ShapeData m_data;
};