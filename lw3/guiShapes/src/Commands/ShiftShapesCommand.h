#pragma once

#include "ICommand.h"
#include "Point.h"
#include "Shape.h"

class ShiftShapesCommand : public ICommand
{
public:
	ShiftShapesCommand(const std::vector<Shape*>& shapes, Bounds offset)
		: m_shapes(shapes)
		, m_offset(offset)
	{
	}

	void Execute() override
	{
		for (auto& shape : m_shapes)
		{
			shape->ShiftBy(m_offset);
		}
	}

	void Unexecute() override
	{
		for (auto& shape : m_shapes)
		{
			shape->ShiftBy(-m_offset);
		}
	}

private:
	std::vector<Shape*> m_shapes;
	Bounds m_offset;
};