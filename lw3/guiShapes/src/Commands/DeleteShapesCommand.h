#pragma once

#include "ICommand.h"
#include "Picture.h"
#include "Shape.h"
#include <vector>

class DeleteShapesCommand : ICommand
{
public:
	DeleteShapesCommand(Picture& picture, const std::vector<Shape>& shapes)
		: m_picture(picture)
		, m_shapes(shapes)
	{
	}

	void Execute() override
	{
	}

	void Unexecute() override
	{
	}

private:
	Picture& m_picture;
	std::vector<Shape> m_shapes;
};