#pragma once

#include "GroupShape.h"
#include "ICommand.h"
#include "Picture.h"
#include <memory>
#include <vector>

class GroupShapesCommand : public ICommand
{
public:
	GroupShapesCommand(Picture& picture, std::vector<size_t>&& indexes)
		: m_picture(picture)
		, m_indexes(std::move(indexes))
		, m_group(std::make_unique<GroupShape>())
		, m_groupPtr(m_group.get())
	{
	}

	[[nodiscard]] GroupShape* GetGroupShape() const noexcept
	{
		return m_groupPtr;
	}

	void Execute() override
	{
		for (auto index : m_indexes)
		{
			m_group->AddShape(m_picture.ExtractShape(index - m_group->GetShapeCount()));
		}

		m_picture.AddShape(std::move(m_group));
	}

	void Unexecute() override
	{
		auto extracted = m_picture.ExtractShape(m_picture.GetShapesCount() - 1);
		m_group.reset(static_cast<GroupShape*>(extracted.release()));

		for (size_t index : m_indexes)
		{
			m_picture.AddShape(m_group->ExtractShape(0), index);
		}
	}

private:
	Picture& m_picture;
	std::vector<size_t> m_indexes;
	std::unique_ptr<GroupShape> m_group;
	GroupShape* m_groupPtr;
};