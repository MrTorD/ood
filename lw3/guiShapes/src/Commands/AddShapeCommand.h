#pragma once

#include "ICommand.h"
#include "Picture.h"

class AddShapeCommand : public ICommand
{
public:
	AddShapeCommand(Picture* picture, std::unique_ptr<Shape> shape, size_t index)
		: m_picture(picture)
		, m_shape(std::move(shape))
		, m_index(index)
	{
	}

	void Execute() override
	{
		m_picture->AddShape(m_shape->Clone(), m_index);
	}

	void Unexecute() override
	{
		m_picture->DeleteShape(m_index);
	}

private:
	Picture* m_picture;
	std::unique_ptr<Shape> m_shape;
	size_t m_index;
};