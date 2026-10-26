#pragma once

#include "AddShapeCommand.h"
#include "DeleteShapesCommand.h"
#include "EditorShape.h"
#include "Picture.h"
#include "PictureSerialiser.h"
#include "ShiftShapesCommand.h"
#include <algorithm>
#include <functional>
#include <vector>

class EditorModel
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;
	using ChangeSignal = boost::signals2::signal<void()>;
	using CommandSink = std::function<void(std::unique_ptr<ICommand>)>;

	EditorModel(Picture* picture, CommandSink commandSink, Bounds& m_canvasBounds)
		: m_picture(picture)
		, m_commandSink(std::move(commandSink))
		, m_canvasBounds(m_canvasBounds)
	{
		SyncShapes();

		m_pictureConnection = m_picture->DoOnChange([this] {
			OnPictureChanged();
		});
	}

	void OnPictureChanged()
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
		for (const auto& shape : GetSelectedShapes() | std::views::reverse)
		{
			if (shape->GetBounds().IsContains(point))
			{
				return shape;
			}
		}

		for (const auto& shape : m_shapes | std::views::reverse)
		{
			if (shape->IsContains(point))
			{
				return shape.get();
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
		auto command = std::make_unique<AddShapeCommand>(
			m_picture,
			std::move(shape),
			m_picture->GetShapesCount());

		m_commandSink(std::move(command));
	}

	void SaveToFile(const std::string& fileName)
	{
		PictureSerialiser serialiser;

		serialiser.Serialise(*m_picture, fileName);
	}

	void BeginShiftShapes(Point point)
	{
		m_activeShapes = GetSelectedShapes();

		for (auto& shape : m_shapes)
		{
			auto dragPoints = shape->GetSelectionBox().GetDragPoints();

			for (auto& dragPoint : dragPoints)
			{
				if (dragPoint.IsContains(point))
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

		std::vector<size_t> indexes;
		for (auto& shape : m_activeShapes)
		{
			auto index = m_picture->GetIndex(&shape->GetDomainShape());
			indexes.push_back(index);
		}

		m_activeShapes.clear();

		auto command = std::make_unique<ShiftShapesCommand>(
			m_picture,
			std::move(indexes),
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
		std::set<size_t> indexes;
		for (const auto& shape : m_shapes)
		{
			if (shape->IsSelected())
			{
				indexes.insert(m_picture->GetIndex(&shape->GetDomainShape()));
			}
		}

		if (!indexes.empty())
		{
			auto command = std::make_unique<DeleteShapesCommand>(m_picture, indexes);
			m_commandSink(std::move(command));
		}
	}

private:
	Picture* m_picture;
	ScopedConnection m_pictureConnection;
	std::vector<std::unique_ptr<EditorShape>> m_shapes;
	std::vector<EditorShape*> m_activeShapes;
	std::function<Bounds(Point)> m_actionFunc;

	CommandSink m_commandSink;
	Bounds& m_canvasBounds;

	float m_minShapeWidth = 50.f;
	float m_minShapeHeight = 50.f;

	void SyncShapes()
	{
		m_shapes.clear();

		while (m_shapes.size() < m_picture->GetShapesCount())
		{
			auto* shape = m_picture->GetShape(m_shapes.size());

			m_shapes.emplace_back(new EditorShape(*shape));
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

		if (x + w > m_canvasBounds.w)
		{
			if (dw == 0)
			{
				x = m_canvasBounds.w - w;
			}
			else
			{
				w = m_canvasBounds.w - x;
			}
		}

		if (y + h > m_canvasBounds.h)
		{
			if (dh == 0)
			{
				y = m_canvasBounds.h - h;
			}
			else
			{
				h = m_canvasBounds.h - y;
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