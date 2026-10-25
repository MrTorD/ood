#pragma once

#include "Ellipse.h"
#include "Picture.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <format>
#include <fstream>
#include <map>
#include <sstream>

class PictureSerialiser : IShapeVisitor
{
public:
	void Serialise(Picture& picture, const std::string& fileName)
	{
		m_file = std::ofstream(fileName);

		for (size_t i = 0; i < picture.GetShapesCount(); i++)
		{
			picture.GetShape(i)->Accept(*this);
		}
	}

	std::unique_ptr<Picture> Deserialise(const std::string& fileName)
	{
		std::ifstream file(fileName);

		if (!file.is_open())
		{
			throw std::invalid_argument("File can't be opened");
		}

		auto pic = std::make_unique<Picture>();

		std::string str;
		while (std::getline(file, str))
		{
			std::stringstream ss(str);

			std::string shapeName;
			ss >> shapeName;

			if (!SHAPES_MAP.contains(shapeName))
			{
				throw std::invalid_argument("Invalid shape name occured");
			}

			auto shapeData = ReadShapeData(ss);
			auto shape = SHAPES_MAP[shapeName](shapeData);

			pic->AddShape(std::move(shape));
		}

		return pic;
	}

private:
	std::ofstream m_file;

	void Visit(const Rectangle& rect) override
	{
		auto [x, y, w, h] = rect.GetBounds();
		m_file << std::format("rectangle {} {} {} {} {}\n", x, y, w, h, rect.GetColor());
	}

	void Visit(const Ellipse& el) override
	{
		auto [x, y] = el.GetCentre();
		m_file << std::format("ellipse {} {} {} {} {}\n", x, y, el.GetRadiusX(), el.GetRadiusY(), el.GetColor());
	}

	void Visit(const Triangle& tr) override
	{
		auto [x, y, w, h] = tr.GetBounds();

		m_file << std::format("triangle {} {} {} {} {}\n", x, y, w, h, tr.GetColor());
	}

	struct ShapeData
	{
		std::vector<float> bounds;
		Color color;
	};

	ShapeData ReadShapeData(std::stringstream& ss)
	{
		std::string str;

		ShapeData shapeData;

		for (int i = 0; i < 4; i++)
		{
			float bound;

			ss >> bound;
			if (ss.fail())
			{
				throw std::invalid_argument("Invalid shape data provided");
			}
			shapeData.bounds.push_back(bound);
		}

		uint32_t color;
		ss >> color;
		if (ss.fail())
		{
			throw std::invalid_argument("Invalid shape data provided");
		}

		shapeData.color = color;

		return shapeData;
	}

	std::unordered_map<std::string, std::function<std::unique_ptr<Shape>(ShapeData)>> SHAPES_MAP{
		{ "rectangle", [](const ShapeData& data) {
			 if (data.bounds.size() != 4)
			 {
				 throw std::invalid_argument("Invalid arguments while reading rectangle data");
			 }

			 return std::make_unique<Rectangle>(Point(data.bounds[0], data.bounds[1]), data.bounds[2], data.bounds[3], data.color);
		 } },

		{ "ellipse", [](const ShapeData& data) {
			 if (data.bounds.size() != 4)
			 {
				 throw std::invalid_argument("Invalid arguments while reading ellipse data");
			 }

			 return std::make_unique<Ellipse>(Point(data.bounds[0], data.bounds[1]), data.bounds[2], data.bounds[3], data.color);
		 } },

		{ "triangle", [](const ShapeData& data) {
			 if (data.bounds.size() != 4)
			 {
				 throw std::invalid_argument("Invalid arguments while reading triangle data");
			 }

			 return std::make_unique<Triangle>(Point(data.bounds[0], data.bounds[1]), data.bounds[2], data.bounds[3], data.color);
		 } },
	};
};