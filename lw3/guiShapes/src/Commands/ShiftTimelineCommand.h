#pragma once

#include "ICommand.h"
#include "Picture.h"
#include <vector>

// TODO: TEMP
#include <format>
#include <iostream>

class ShiftTimelineCommand : public ICommand
{
public:
	ShiftTimelineCommand(Picture* picture, std::vector<size_t>&& shapeIndexes, int liftLayersCount, Point offset)
		: m_picture(picture)
		, m_shapeIndexes(std::move(shapeIndexes))
		, m_liftLayersCount(liftLayersCount)
		, m_offset(offset)
	{
	}

	void Execute() override
	{
		for (auto index : m_shapeIndexes)
		{
			std::cout << std::format("Lifting: {} to {}\n", index, m_liftLayersCount);
			m_picture->LiftShape(index, m_liftLayersCount);
		}
	}

	void Unexecute() override
	{
		for (auto index : m_shapeIndexes)
		{
			std::cout << std::format("Unlifting: {} to {}\n", index + m_liftLayersCount, -m_liftLayersCount);

			m_picture->LiftShape(m_liftLayersCount + index, -m_liftLayersCount);
		}
	}

private:
	Picture* m_picture;
	std::vector<size_t> m_shapeIndexes;
	int m_liftLayersCount;
	Point m_offset;
};