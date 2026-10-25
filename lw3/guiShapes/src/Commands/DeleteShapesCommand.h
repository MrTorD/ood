#pragma once

#include "ICommand.h"
#include "Picture.h"
#include "Shape.h"
#include <memory>
#include <set>

class DeleteShapesCommand : public ICommand
{
public:
	DeleteShapesCommand(Picture* picture, const std::set<size_t>& indexes)
		: m_picture(picture)
	{
		for (auto index : indexes)
		{
			auto shape = picture->GetShape(index)->Clone();
			m_shapes.emplace(index, std::move(shape));
		}
	}

	void Execute() override
	{
		for (auto&& [index, _] : m_shapes)
		{
			m_picture->DeleteShape(index);
		}
	}

	void Unexecute() override
	{
		for (auto&& [index, shape] : m_shapes)
		{
			m_picture->AddShape(shape->Clone(), index);
		}
	}

private:
	Picture* m_picture;
	std::unordered_map<size_t, std::unique_ptr<Shape>> m_shapes;
};