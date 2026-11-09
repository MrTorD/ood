#pragma once

#include "ICommand.h"
#include "Picture.h"
#include <vector>

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
			auto newTimeline = m_picture->GetShape(index)->GetTimeline() + m_offset;
			m_picture->SetShapeTimeline(index, newTimeline);
			if (m_liftLayersCount != 0)
			{
				m_picture->LiftShape(index, m_liftLayersCount);
			}
		}
	}

	void Unexecute() override
	{
		for (auto index : m_shapeIndexes)
		{
			if (m_liftLayersCount != 0)
			{
				m_picture->LiftShape(m_liftLayersCount + index, -m_liftLayersCount);
			}
			auto newTimeline = m_picture->GetShape(index)->GetTimeline() - m_offset;
			m_picture->SetShapeTimeline(index, newTimeline);
		}
	}

private:
	Picture* m_picture;
	std::vector<size_t> m_shapeIndexes;
	int m_liftLayersCount;
	Point m_offset;
};