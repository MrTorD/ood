#pragma once

#include "ICommand.h"
#include "ShapeData.h"
#include "ParsingUtils.h"

class AddShapeCommand : public ICommand
{
public:
	AddShapeCommand(std::stringstream& ss)
	{
		m_data.id = ReadStringStrictly(ss);
		m_data.color = ReadColor(ss);
		m_data.shapeType = ReadShapeType(ReadStringStrictly(ss));

		if (m_data.shapeType == ShapeType::Unknown)
		{
			throw CommandParseError("Invalid shape provided");
		}

		ReadConcreteShapeData(ss, m_data);
	}

	void Execute(Picture& picture) override
	{
		picture.AddShape(m_data.id, CreateGeometry(m_data), m_data.color);
	}

private:
	ShapeData m_data;
};