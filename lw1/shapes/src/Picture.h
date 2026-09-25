#pragma once

#include "NotFoundError.h"
#include "PictureInvalidOperation.h"
#include "Shape.h"
#include <list>
#include <unordered_map>

class Picture
{
public:
	Picture(std::unique_ptr<ICanvas> canvas)
	{
		m_canvas = std::move(canvas);
	}

	void AddShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Color color)
	{
		if (m_idMap.contains(id))
		{
			throw PictureInvalidOperation("Shape with id: " + id + " already exists");
		}

		m_shapes.push_back({ id, std::move(geometry), color });
		m_idMap[id] = --m_shapes.end();
	}

	void MoveShape(const std::string& id, double dx, double dy)
	{
		CheckIdExistance(id);

		Shape& shape = *m_idMap[id];
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
		// [x]: Вынести в validateId();
		CheckIdExistance(id);

		m_shapes.erase(m_idMap[id]);
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
		CheckIdExistance(id);

		Shape& shape = *m_idMap[id];
		shape.SetColor(newColor);
	}

	void CloneShape(const std::string& id, const std::string& newId)
	{
		CheckIdExistance(id);

		Shape& shape = *m_idMap[id];
		m_shapes.push_back(shape.Clone(newId));
	}

	void ChangeShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry)
	{
		CheckIdExistance(id);

		Shape& shape = *m_idMap[id];
		shape.SetGeometry(std::move(geometry));
	}

	void DrawShape(const std::string& id)
	{
		CheckIdExistance(id);

		Shape& shape = *m_idMap[id];
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

	std::list<Shape> m_shapes;
	// [x]: Подумать над вектором и инвалидацией ссылок при клонировнии (вектор перемещается в памяти)
	std::unordered_map<std::string, std::list<Shape>::iterator> m_idMap;
	std::unique_ptr<ICanvas> m_canvas;
};