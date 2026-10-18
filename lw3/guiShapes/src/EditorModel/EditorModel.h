#pragma once

#include "AddShapeCommand.h"
#include "EditorShape.h"
#include "Picture.h"
#include "PictureSerialiser.h"
#include "ShiftShapesCommand.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

class EditorModel
{
public:
	using CommandSink = std::function<void(std::unique_ptr<ICommand>)>;

	explicit EditorModel(Picture& picture, CommandSink commandSink, float width, float height)
		: m_picture(picture)
		, m_commandSink(std::move(commandSink))
		, m_width(width)
		, m_height(height)
	{
		SyncShapes();
	}

	size_t GetShapesCount() const
	{
		return m_shapes.size();
	}

	EditorShape& GetShape(size_t index)
	{
		return *m_shapes.at(index);
	}

	const EditorShape& GetShape(size_t index) const
	{
		return *m_shapes.at(index);
	}

	EditorShape* FindShapeAt(Point point)
	{
		for (const auto& shape : m_shapes | std::views::reverse)
		{
			// TODO: Simplify
			if (shape->IsSelected())
			{
				if (shape->GetBounds().HitTest(point))
				{
					return shape.get();
				}
			}
			else
			{
				if (shape->HitTest(point))
				{
					return shape.get();
				}
			}
		}

		return nullptr;
	}

	std::vector<EditorShape*> GetSelectedShapes() const
	{
		std::vector<EditorShape*> selectedShapes;

		for (auto& shape : m_shapes)
		{
			if (shape->IsSelected())
			{
				selectedShapes.push_back(shape.get());
			}
		}

		return selectedShapes;
	}

	void AddShape(std::unique_ptr<Shape> shape)
	{
		m_picture.AddShape(std::move(shape));

		// auto command = std::make_unique<AddShapeCommand>();
		// m_commandSink(std::move(command));

		SyncShapes();
	}

	void SaveToFile(const std::string& fileName)
	{
		PictureSerialiser serialiser;

		serialiser.Serialise(m_picture, fileName);
	}

	void LoadFromFile(const std::string& fileName)
	{
		PictureSerialiser serialiser;

		try
		{
			auto newPicture = serialiser.Deserialise(fileName);

			std::swap(m_picture, newPicture);
			SyncShapes();
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << e.what() << '\n';
		}
	}

	void BeginShiftShapes(Point point)
	{
		m_activeShapes = GetSelectedShapes();

		// TODO: simplify
		for (auto& shape : m_shapes)
		{
			auto dragPoints = shape->GetSelectionBox().GetDragPoints();

			for (auto& dragPoint : dragPoints)
			{
				if (dragPoint.HitTest(point))
				{
					m_actionFunc = dragPoint.GetOffset;
					return;
				}
			}
		}

		m_actionFunc = [](Point point) {
			return Bounds(point.x, point.y, 0, 0);
		};
	}

	void UpdateShiftShapes(Point point)
	{
		auto offset = m_actionFunc(point);

		for (auto& shape : m_activeShapes)
		{
			ClampOffset(shape->GetDomainShape().GetBounds(), offset);
		}

		for (auto& shape : m_activeShapes)
		{
			shape->SetPreviewOffset(offset);
		}
	}

	void CommitShiftShapes()
	{
		if (m_activeShapes.empty())
		{
			return;
		}

		auto offset = m_activeShapes[0]->GetPreviewOffset();

		for (auto& shape : m_activeShapes)
		{
			shape->ResetPreview();
		}

		if (offset == Bounds(0, 0, 0, 0))
		{
			return;
		}

		std::vector<Shape*> domainShapes;
		for (auto& shape : m_activeShapes)
		{
			domainShapes.push_back(&shape->GetDomainShape());
		}

		m_activeShapes.clear();

		auto command = std::make_unique<ShiftShapesCommand>(
			domainShapes,
			offset);

		m_commandSink(std::move(command));
	}

	void UnselectAll()
	{
		for (const auto& shape : m_shapes)
		{
			shape->SetSelected(false);
		}
	}

	void DeleteSelectedShapes()
	{
		// std::vector<Shape*> deleteShapes;
		for (const auto& shape : m_shapes)
		{
			if (shape->IsSelected())
			{
				m_picture.DeleteShape(&shape->GetDomainShape());
				// deleteShapes.push_back(&shape->GetDomainShape());
			}
		}

		// auto command = std::make_unique<DeleteShapesCommand>(deleteShapes);
		SyncShapes();
	}

private:
	Picture& m_picture;
	std::vector<std::unique_ptr<EditorShape>> m_shapes;
	std::vector<EditorShape*> m_activeShapes;
	std::function<Bounds(Point)> m_actionFunc;

	CommandSink m_commandSink;
	float m_width;
	float m_height;

	float m_minShapeWidth = 50.f;
	float m_minShapeHeight = 50.f;

	void SyncShapes()
	{
		m_shapes.clear();

		while (m_shapes.size() < m_picture.GetShapesCount())
		{
			auto& shape = m_picture.GetShape(m_shapes.size());

			m_shapes.emplace_back(new EditorShape(shape));
		}
	}

	void ClampOffset(Bounds shapeBounds, Bounds& offset)
	{
		auto [x0, y0, w0, h0] = shapeBounds;
		auto [dx, dy, dw, dh] = offset;

		float x = x0 + dx;
		float y = y0 + dy;
		float w = w0 + dw;
		float h = h0 + dh;

		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;

		if (x + w > m_width)
		{
			if (dw == 0)
			{
				x = m_width - w;
			}
			else
			{
				w = m_width - x;
			}
		}

		if (y + h > m_height)
		{
			if (dh == 0)
			{
				y = m_height - h;
			}
			else
			{
				h = m_height - y;
			}
		}

		offset.x = x - x0;
		offset.y = y - y0;
		offset.w = w - w0;
		offset.h = h - h0;

		if (w <= m_minShapeWidth)
		{
			offset.w = m_minShapeWidth - w0;
			offset.x += offset.x > 0
				? dw - offset.w
				: 0;
		}

		if (h <= m_minShapeHeight)
		{
			offset.h = m_minShapeHeight - h0;
			offset.y += offset.y > 0
				? dh - offset.h
				: 0;
		}

		if (dx != 0 && dw != 0)
		{
			auto minAbsOffset = std::min(std::abs(offset.x), std::abs(offset.w));

			offset.x = offset.x > 0
				? minAbsOffset
				: -minAbsOffset;

			offset.w = offset.w > 0
				? minAbsOffset
				: -minAbsOffset;
		}

		if (dy != 0 && dh != 0)
		{
			auto minAbsOffset = std::min(std::abs(offset.y), std::abs(offset.h));

			offset.y = offset.y > 0
				? minAbsOffset
				: -minAbsOffset;

			offset.h = offset.h > 0
				? minAbsOffset
				: -minAbsOffset;
		}
	}
};