#pragma once

#include "GroupShape.h"
#include "ICommand.h"
#include "Picture.h"

class UngroupShapesCommand final : public ICommand
{
public:
	UngroupShapesCommand(Picture& picture, size_t groupIndex)
		: m_picture(picture)
		, m_groupIndex(groupIndex)
	{
		auto* group = dynamic_cast<GroupShape*>(&picture.GetShape(groupIndex));
		if (!group || group->GetShapeCount() == 0)
		{
			throw std::invalid_argument("Expected a non-empty top-level group");
		}

		m_children.reserve(group->GetShapeCount());
		for (size_t i = 0; i < group->GetShapeCount(); ++i)
		{
			m_children.push_back(&group->GetShape(i));
		}
	}

	[[nodiscard]] const std::vector<IShape*>& GetChildrenShapes() const noexcept
	{
		return m_children;
	}

	void Execute() override
	{
		auto extracted = m_picture.ExtractShape(m_groupIndex);
		m_group.reset(static_cast<GroupShape*>(extracted.release()));

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			m_picture.AddShape(m_group->ExtractShape(0), m_groupIndex + i);
		}
	}

	void Unexecute() override
	{
		for (size_t i = m_children.size(); i > 0; i--)
		{
			m_group->AddShape(m_picture.ExtractShape(m_groupIndex + i - 1), 0);
		}

		m_picture.AddShape(std::move(m_group), m_groupIndex);
	}

private:
	Picture& m_picture;
	size_t m_groupIndex;
	std::unique_ptr<GroupShape> m_group;
	std::vector<IShape*> m_children;
};