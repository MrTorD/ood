#pragma once

#include "BoostShape.h"
#include "Color.h"
#include "NotFoundError.h"
#include "PictureInvalidOperation.h"
#include <boost/signals2.hpp>
#include <iostream>
#include <list>
#include <unordered_map>

using boost::signals2::scoped_connection;

class BoostPicture
{
public:
	BoostPicture(std::unique_ptr<ICanvas> canvas)
	{
		m_canvas = std::move(canvas);
	}

	BoostShape& GetShape(const std::string& id)
	{
		return *m_idMap[id];
	}

	unsigned GetShapesCount() const
	{
		unsigned count = 0;

		for (const auto& shape : m_shapes)
		{
			count++;
		}

		return count;
	}

	void AddShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Color color)
	{
		if (m_idMap.contains(id))
		{
			throw PictureInvalidOperation("Shape with id: " + id + " already exists");
		}

		m_shapes.push_back({ id, std::move(geometry), color });
		m_idMap[id] = --m_shapes.end();

		auto& shape = *m_idMap[id];
		m_colorChangeSubs.push_back(shape.OnColorChanged([&](const std::string& id, Color color) {
			m_onColorChanged(id, color);
		}));

		m_moveSubs.push_back(shape.OnShapeMoved([&](const std::string& id, double dx, double dy) {
			m_onShapeMoved(id, dx, dy);
		}));

		m_onShapeAdded(*m_idMap[id]);
	}

	scoped_connection OnShapeAdded(std::function<void(BoostShape&)> observer)
	{
		return m_onShapeAdded.connect(observer);
	}

	scoped_connection OnShapeDeleted(std::function<void(const std::string&)> observer)
	{
		return m_onShapeDeleted.connect(observer);
	}

	scoped_connection OnShapeMoved(std::function<void(const std::string&, double, double)> observer)
	{
		return m_onShapeMoved.connect(observer);
	}

	scoped_connection OnColorChanged(std::function<void(const std::string&, Color)> observer)
	{
		return m_onColorChanged.connect(observer);
	}

	void MoveShape(std::string& id, double dx, double dy)
	{
		CheckIdExistance(id);

		auto& shape = *m_idMap[id];
		shape.Move(dx, dy);
	}

	void MovePicture(double dx, double dy)
	{
		for (auto& shape : m_shapes)
		{
			shape.Move(dx, dy);
		}
	}

	void DeleteShape(std::string& id)
	{
		CheckIdExistance(id);

		m_shapes.erase(m_idMap[id]);
		m_idMap.erase(id);

		m_onShapeDeleted(id);
	}

	void PrintShapes(std::ostream& output)
	{
		unsigned count = 1;

		for (auto& shape : m_shapes)
		{
			output << count << ' ';
			shape.Print(output);
			output << "\n";
			count++;
		}
	}

	void ChangeShapeColor(const std::string& id, Color newColor)
	{
		CheckIdExistance(id);

		auto& shape = *m_idMap[id];
		shape.SetColor(newColor);
	}

	void ChangeShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry)
	{
		CheckIdExistance(id);

		auto& shape = *m_idMap[id];
		shape.SetGeometry(std::move(geometry));
	}

	void DrawShape(const std::string& id)
	{
		CheckIdExistance(id);

		auto& shape = *m_idMap[id];
		shape.Draw(*m_canvas);
	}

	void DrawPicture()
	{
		for (const auto& shape : m_shapes)
		{
			shape.Draw(*m_canvas);
		}
	}

private:
	void CheckIdExistance(const std::string& id) const
	{
		if (!m_idMap.contains(id))
		{
			throw NotFoundError("Shape with id: " + id + " doesn't exist");
		}
	}

	std::list<BoostShape> m_shapes;
	std::unordered_map<std::string, std::list<BoostShape>::iterator> m_idMap;
	std::unique_ptr<ICanvas> m_canvas;

	std::list<scoped_connection> m_moveSubs;
	std::list<scoped_connection> m_colorChangeSubs;

	boost::signals2::signal<void(BoostShape&)> m_onShapeAdded;
	boost::signals2::signal<void(const std::string&)> m_onShapeDeleted;
	boost::signals2::signal<void(const std::string&, double, double)> m_onShapeMoved;
	boost::signals2::signal<void(const std::string&, Color)> m_onColorChanged;
};