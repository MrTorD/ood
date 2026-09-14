#pragma once

#include "Errors/NotFoundError.h"
#include "Errors/PictureInvalidOperation.h"
#include "Shapes/Shape.h"
#include <unordered_map>
#include <vector>

class Picture
{
public:
	Picture(std::unique_ptr<ICanvas> canvas)
	{
		m_canvas = std::move(canvas);
	}

	void AddShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Rect bounds, Color color)
	{
		if (m_idMap.contains(id))
		{
			throw PictureInvalidOperation("Shape with id: " + id + " already exists");
		}

		m_shapes.push_back({ id, std::move(geometry), bounds, color });
		m_idMap[id] = m_shapes.size() - 1;
	}

	void MoveShape(const std::string& id, double dx, double dy)
	{
		if (!m_idMap.contains(id))
		{
			throw NotFoundError("Shape with id: " + id + " doesn't exist");
		}

		Shape& shape = m_shapes[m_idMap[id]];
		shape.Move(dx, dy);
	}

	void MovePicture(double dx, double dy)
	{
		for (auto& shape : m_shapes)
		{
			shape.Move(dx, dy);
		}
	}

	void DeleteShape(const std::string& id)
	{
		if (!m_idMap.contains(id))
		{
			throw NotFoundError("Shape with id: " + id + " doesn't exist");
		}

		m_shapes.erase(m_shapes.begin() + m_idMap[id]);
		m_idMap.erase(id);
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
		if (!m_idMap.contains(id))
		{
			throw NotFoundError("Shape with id: " + id + " doesn't exist");
		}

		Shape& shape = m_shapes[m_idMap[id]];
		shape.SetColor(newColor);
	}

	void ChangeShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Rect newBounds)
	{
		if (!m_idMap.contains(id))
		{
			throw NotFoundError("Shape with id: " + id + " doesn't exist");
		}

		Shape& shape = m_shapes[m_idMap[id]];
		shape.SetGeometry(std::move(geometry));
		shape.SetBounds(newBounds);
	}

	void DrawShape(const std::string& id)
	{
		if (!m_idMap.contains(id))
		{
			throw NotFoundError("Shape with id: " + id + " doesn't exist");
		}

		Shape& shape = m_shapes[m_idMap[id]];
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
	std::vector<Shape> m_shapes;
	std::unordered_map<std::string, unsigned> m_idMap;
	std::unique_ptr<ICanvas> m_canvas;
};