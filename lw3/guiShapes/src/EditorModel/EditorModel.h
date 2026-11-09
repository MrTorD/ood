#pragma once

#include "AddShapeCommand.h"
#include "CanvasMode.h"
#include "DeleteShapesCommand.h"
#include "EditorShape.h"
#include "Picture.h"
#include "PictureSerializer.h"
#include "ShiftShapesCommand.h"
#include "ShiftTimelineCommand.h"
#include <algorithm>
#include <functional>
#include <vector>

class EditorModel
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;
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

	Bounds GetBounds() const
	{
		return { 0, 0, m_canvasBounds.w, m_canvasBounds.h };
	}

	size_t GetShapesCount() const
	{
		return m_shapes.size();
	}

	EditorShape& GetShape(size_t index)
	{
		return *m_shapes.at(index);
	}

	CanvasMode GetCanvasMode() const
	{
		return m_canvasMode;
	}

	void SetCanvasMode(CanvasMode mode)
	{
		m_canvasMode = mode;
	}

	const EditorShape& GetShape(size_t index) const
	{
		return *m_shapes.at(index);
	}

	Point GetMinimalShapeSize() const
	{
		return { m_minShapeWidth, m_minShapeHeight };
	}

	float GetCurrentTime() const
	{
		return m_currTime;
	}

	void SetCurrentTime(float time)
	{
		m_currTime = time;
	}

	size_t GetIndex(const EditorShape& shape) const
	{
		return m_picture->GetIndex(&shape.GetDomainShape());
	}

	bool IsShapeTimeBetween(const EditorShape* shape) const
	{
		auto [t, _, dt, _] = shape->GetTimeline();

		return t <= m_currTime && m_currTime <= t + dt;
	}

	EditorShape* FindShapeAt(Point point)
	{
		for (const auto& shape : GetSelectedShapes() | std::views::reverse)
		{
			if (shape->GetSelectionBox().IsContains(point) && IsShapeTimeBetween(shape))
			{
				return shape;
			}
		}

		for (const auto& shape : m_shapes | std::views::reverse)
		{
			if (shape->IsContains(point) && IsShapeTimeBetween(shape.get()))
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
				selectedShapes.emplace_back(shape.get());
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

	void SaveToFile(const std::string& fileName) const
	{
		PictureSerializer serializer;

		serializer.Serialize(*m_picture, fileName);
	}

	void BeginShiftShapes(Point point)
	{
		m_activeShapes = GetSelectedShapes();

		if (!TrySetAction(
				point,
				[](const EditorShape& sh) {
					return sh.GetSelectionBox().GetDragPoints();
				}))
		{
			m_actionFunc = [](Point point) {
				return Bounds(point.x, point.y, 0, 0);
			};
		}
	}

	void BeginTimelineShift(Point point)
	{
		m_activeShapes = GetSelectedShapes();

		if (!TrySetAction(
				point,
				[this](const EditorShape& shape) -> std::vector<DragPoint> {
					auto index = GetIndex(shape);
					return shape.GetTimelineBox(m_shapes.size() - 1 - index).GetDragPoints();
				}))
		{
			m_actionFunc = [](Point point) {
				return Bounds(point.x, point.y, 0, 0);
			};
		}
	}

	void BeginCreateShape(Shape& shape)
	{
		m_shapes.emplace_back(new EditorShape(shape));
		m_activeShapes = { m_shapes.back().get() };

		m_actionFunc = [](Point point) {
			return Bounds(0, 0, point.x, point.y);
		};
	}

	void UpdateTimelineShift(Point point)
	// TODO: Think about
	{
		auto [dx, dy, dw, dh] = m_actionFunc(point);

		for (auto& shape : m_activeShapes)
		{
			auto [x, w] = shape->GetDomainShape().GetTimeline();
			auto [newX, newW] = ClampDimension(x, w, dx, dw, 1000, m_minShapeDuration); // TODO: Min & Max offset
			dx = newX;
			dw = newW;
		}

		for (auto& shape : m_activeShapes)
		{
			shape->SetTimelineOffset({ dx, dy, dw, dh });
		}
	}

	void CommitTimelineShift()
	{
		if (m_activeShapes.empty())
		{
			return;
		}

		auto timelineOffset = m_activeShapes[0]->GetTimelineOffset();

		std::vector<size_t> indexes;
		int liftLayersCount = -timelineOffset.y / TimelineBox::GetTotalHeight();

		for (auto* shape : m_activeShapes)
		{
			shape->ResetTimelineShift();
			auto index = GetIndex(*shape);
			indexes.push_back(index);
			ClampLiftLayers(index, liftLayersCount);
		}
		m_activeShapes.clear();

		if (liftLayersCount == 0 && timelineOffset.x == 0 && timelineOffset.w == 0)
		{
			return;
		}

		auto command = std::make_unique<ShiftTimelineCommand>(
			m_picture,
			std::move(indexes),
			liftLayersCount,
			Point(timelineOffset.x, timelineOffset.w));
			
		m_commandSink(std::move(command));
	}

	void UpdateActiveShapes(Point point)
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

	void CommitActiveShapes()
	{
		if (m_activeShapes.empty())
		{
			return;
		}

		auto offset = m_activeShapes[0]->GetPreviewOffset();
		if (offset == Bounds(0, 0, 0, 0))
		{
			return;
		}

		for (auto& shape : m_activeShapes)
		{
			shape->ResetPreview();
		}

		auto command = m_canvasMode == CanvasMode::SelectShapes
			? CreateShiftCommand(offset)
			: CreateShapeCommand(offset);

		m_activeShapes.clear();
		m_canvasMode = CanvasMode::SelectShapes;

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
	CanvasMode m_canvasMode = CanvasMode::SelectShapes;

	float m_currTime = 0;
	float m_minShapeDuration = 20;
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

	std::unique_ptr<ICommand> CreateShiftCommand(Bounds offset)
	{
		std::vector<size_t> indexes;
		for (auto& shape : m_activeShapes)
		{
			auto index = m_picture->GetIndex(&shape->GetDomainShape());
			indexes.push_back(index);
		}

		m_activeShapes.clear();

		return std::make_unique<ShiftShapesCommand>(
			m_picture,
			std::move(indexes),
			offset);
	}

	std::unique_ptr<ICommand> CreateShapeCommand(Bounds offset)
	{
		auto& shape = m_activeShapes.back()->GetDomainShape();
		shape.ShiftBy(offset);

		return std::make_unique<AddShapeCommand>(
			m_picture,
			std::move(shape.Clone()),
			m_picture->GetShapesCount());
	}

	bool TrySetAction(
		Point point,
		const std::function<std::vector<DragPoint>(const EditorShape&)>& GetDragPoints)
	{
		for (const auto& shape : m_activeShapes)
		{
			auto dragPoints = GetDragPoints(*shape);

			if (auto it = std::ranges::find_if(dragPoints, [&point](const DragPoint& p) {
					return p.IsContains(point);
				});
				it != dragPoints.end())
			{
				m_actionFunc = it->GetOffset;
				return true;
			}
		}

		return false;
	}

	void ClampLiftLayers(size_t index, int& liftLayersCount)
	{
		int newIndex = index + liftLayersCount;

		if (newIndex < 0)
		{
			liftLayersCount = -index;
		}
		else if (newIndex >= m_picture->GetShapesCount())
		{
			liftLayersCount = m_picture->GetShapesCount() - 1 - index;
		}
	}

	void ClampOffset(Bounds shapeBounds, Bounds& offset)
	{
		auto [x0, y0, w0, h0] = shapeBounds;
		auto [dx, dy, dw, dh] = offset;

		auto [x, w] = ClampDimension(x0, w0, dx, dw, m_canvasBounds.w, m_minShapeWidth);
		auto [y, h] = ClampDimension(y0, h0, dy, dh, m_canvasBounds.h, m_minShapeHeight);

		offset = { x, y, w, h };
	}

	static std::pair<float, float> ClampDimension(float x0, float w0, float dx, float dw, float maxX, float minW)
	{
		std::pair<float, float> offset(dx, dw);

		float x = x0 + dx;
		float w = w0 + dw;

		x = x >= 0 ? x : 0;

		if (x + w > maxX)
		{
			if (dw == 0)
			{
				x = maxX - w;
			}
			else
			{
				w = maxX - x;
			}
		}

		offset.first = x - x0;
		offset.second = w - w0;

		if (w <= minW)
		{
			offset.second = minW - w0;
			offset.first += offset.first > 0
				? dw - offset.second
				: 0;
		}

		if (dx != 0 && dw != 0)
		{
			auto minAbsOffset = std::min(std::abs(offset.first), std::abs(offset.second));

			offset.first = offset.first > 0
				? minAbsOffset
				: -minAbsOffset;

			offset.second = offset.second > 0
				? minAbsOffset
				: -minAbsOffset;
		}

		return offset;
	}
};