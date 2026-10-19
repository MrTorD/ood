#pragma once

#include "ICommand.h"
#include "Picture.h"
#include "Shape.h"
#include <memory>
#include <unordered_map>

class DeleteShapesCommand : public ICommand
{
public:
	DeleteShapesCommand(Picture& picture, std::unordered_map<size_t, std::unique_ptr<Shape>>&& shapes)
		: m_picture(picture)
		, m_shapes(std::move(shapes))
	{
	}

	void Execute() override
	{
		for (auto&& [index, _] : m_shapes)
		{
			m_picture.DeleteShape(index);
		}
	}

	void Unexecute() override
	{
		for (auto&& [index, shape] : m_shapes)
		{
			m_picture.AddShape(shape->Clone(), index);
		}
	}

private:
	Picture& m_picture;
	std::unordered_map<size_t, std::unique_ptr<Shape>> m_shapes;
};