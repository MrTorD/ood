#pragma once

#include "NotFoundError.h"
#include "PictureInvalidOperation.h"
#include "Shape.h"
#include "SubjectImpl.h"
#include "Subscribtion.h"
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>

class Picture : public SubjectImpl<Picture>
	, public IObserver<Shape>
{
public:
	Picture(std::unique_ptr<ICanvas> canvas)
	{
		m_canvas = std::move(canvas);
	}

	void Update() override
	{
		NotifyObservers();
	}

	void CancelSubscribtion(Shape* subject) override
	{
		auto subIt = std::find_if(m_subscribtions.begin(), m_subscribtions.end(), [&](const Subscribtion<Shape>& sub) {
			return sub.GetSubject() == subject;
		});

		if (subIt == m_subscribtions.end())
		{
			return;
		}

		m_subscribtions.erase(subIt);
	}

	Shape& GetShape(const std::string& id)
	{
		return *m_idMap[id];
	}

	unsigned GetShapesCount() const
	{
		return m_shapes.size();
	}

	void AddShape(const std::string& id, std::unique_ptr<IShapeGeometry> geometry, Color color)
	{
		if (m_idMap.contains(id))
		{
			throw PictureInvalidOperation("Shape with id: " + id + " already exists");
		}

		m_shapes.push_back({ id, std::move(geometry), color });
		m_idMap[id] = --m_shapes.end();

		m_subscribtions.push_back(m_idMap[id]->Subscribe(this));

		NotifyObservers();
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
		CheckIdExistance(id);

		m_shapes.erase(m_idMap[id]);
		m_idMap.erase(id);

		NotifyObservers();
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
	std::unordered_map<std::string, std::list<Shape>::iterator> m_idMap;

	std::list<Subscribtion<Shape>> m_subscribtions;
	// [x] Избавиться от subscribtionIdMap. Подписки не должны инвалидироваться при смерти субъекта

	std::unique_ptr<ICanvas> m_canvas;
};