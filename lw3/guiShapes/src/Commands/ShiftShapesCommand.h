#pragma once

#include "ICommand.h"
#include "Picture.h"

class ShiftShapesCommand : public ICommand
{
public:
	ShiftShapesCommand(Picture& picture, std::vector<size_t>& indexes, Bounds offset)
		: m_picture(picture)
		, m_indexes(std::move(indexes))
		, m_offset(offset)
	{
	}

	void Execute() override
	{
		for (auto index : m_indexes)
		{
			auto* shape = m_picture.GetShape(index);
			shape->ShiftBy(m_offset);
		}
	}

	void Unexecute() override
	{
		for (auto index : m_indexes)
		{
			auto* shape = m_picture.GetShape(index);
			shape->ShiftBy(-m_offset);
		}
	}

private:
	Picture& m_picture;
	std::vector<size_t> m_indexes;
	Bounds m_offset;
};