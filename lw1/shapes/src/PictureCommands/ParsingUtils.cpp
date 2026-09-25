#include "ParsingUtils.h"
#include "CircleGeometry.h"
#include "LineGeometry.h"
#include "RectangleGeometry.h"
#include "ShapeType.h"
#include "TextGeometry.h"
#include "TriangleGeometry.h"
#include <functional>
#include <memory>

#include "AddShapeCommand.h"
#include "ChangeColorCommand.h"
#include "ChangeShapeCommand.h"
#include "DeleteShapeCommand.h"
#include "DrawPictureCommand.h"
#include "DrawShapeCommand.h"
#include "ListCommand.h"
#include "MovePictureCommand.h"
#include "MoveShapeCommand.h"
#include "CloneShapeCommand.h"

void ReadLineData(std::stringstream& ss, ShapeData& data);
void ReadCircleData(std::stringstream& ss, ShapeData& data);
void ReadRectangleData(std::stringstream& ss, ShapeData& data);
void ReadTriangleData(std::stringstream& ss, ShapeData& data);
void ReadTextData(std::stringstream& ss, ShapeData& data);

std::unordered_map<std::string, std::function<std::unique_ptr<ICommand>(std::stringstream&)>> COMMANDS_MAP = {
	{ "AddShape", [](std::stringstream& ss) { return std::make_unique<AddShapeCommand>(ss); } },
	{ "MoveShape", [](std::stringstream& ss) { return std::make_unique<MoveShapeCommand>(ss); } },
	{ "MovePicture", [](std::stringstream& ss) { return std::make_unique<MovePictureCommand>(ss); } },
	{ "DeleteShape", [](std::stringstream& ss) { return std::make_unique<DeleteShapeCommand>(ss); } },
	{ "List", [](std::stringstream& ss) { return std::make_unique<ListCommand>(); } },
	{ "ChangeColor", [](std::stringstream& ss) { return std::make_unique<ChangeColorCommand>(ss); } },
	{ "ChangeShape", [](std::stringstream& ss) { return std::make_unique<ChangeShapeCommand>(ss); } },
	{ "DrawShape", [](std::stringstream& ss) { return std::make_unique<DrawShapeCommand>(ss); } },
	{ "DrawPicture", [](std::stringstream& ss) { return std::make_unique<DrawPictureCommand>(ss); } },
	{ "CloneShape", [](std::stringstream& ss) { return std::make_unique<CloneShapeCommand>(ss); } }
};

std::unordered_map<std::string, ShapeType> SHAPE_TYPES = {
	{ "LineSegment", ShapeType::LineSegment },
	{ "Circle", ShapeType::Circle },
	{ "Rectangle", ShapeType::Rectangle },
	{ "Triangle", ShapeType::Triangle },
	{ "Text", ShapeType::Text },
};

std::unordered_map<ShapeType, std::function<void(std::stringstream&, ShapeData&)>> SHAPE_READERS = {
	{ ShapeType::LineSegment, [](std::stringstream& ss, ShapeData& data) { return ReadLineData(ss, data); } },
	{ ShapeType::Circle, [](std::stringstream& ss, ShapeData& data) { return ReadCircleData(ss, data); } },
	{ ShapeType::Rectangle, [](std::stringstream& ss, ShapeData& data) { return ReadRectangleData(ss, data); } },
	{ ShapeType::Triangle, [](std::stringstream& ss, ShapeData& data) { return ReadTriangleData(ss, data); } },
	{ ShapeType::Text, [](std::stringstream& ss, ShapeData& data) { return ReadTextData(ss, data); } },
};

std::unique_ptr<ICommand> ReadCommand(std::stringstream& ss)
{
	std::string command;
	ss >> command;

	if (!COMMANDS_MAP.contains(command))
	{
		throw CommandParseError("This command doesn't exist");
	}

	return COMMANDS_MAP[command](ss);
}

std::unique_ptr<IShapeGeometry> CreateGeometry(const ShapeData& args)
{
	switch (args.shapeType)
	{
	case ShapeType::Circle:
		return std::make_unique<CircleGeometry>(args.bounds);
	case ShapeType::LineSegment:
		return std::make_unique<LineGeometry>(args.bounds);
	case ShapeType::Rectangle:
		return std::make_unique<RectangleGeometry>(args.bounds);
	case ShapeType::Text:
		return std::make_unique<TextGeometry>(args.bounds, args.text, args.fontSize);
	case ShapeType::Triangle:
		return std::make_unique<TriangleGeometry>(args.bounds);
	default:
		throw CommandParseError("This shape doesn't exist");
	}
}

ShapeType ReadShapeType(const std::string& str)
{
	if (!SHAPE_TYPES.contains(str))
	{
		throw CommandParseError("This shape doesn't exist");
	}

	return SHAPE_TYPES[str];
}

void ReadConcreteShapeData(std::stringstream& ss, ShapeData& data)
{
	if (!SHAPE_READERS.contains(data.shapeType))
	{
		throw CommandParseError("ShapeType doesn't exist");
	}

	return SHAPE_READERS[data.shapeType](ss, data);
}

void ReadLineData(std::stringstream& ss, ShapeData& data)
{
	Point start = ReadPoint(ss);
	Point end = ReadPoint(ss);

	data.bounds = { start, end.x - start.x, end.y - start.y };
}

void ReadCircleData(std::stringstream& ss, ShapeData& data)
{
	auto [cx, cy] = ReadPoint(ss);
	double radius = ReadDouble(ss);

	data.bounds = { { cx - (radius / 2) }, radius, radius };
}

void ReadRectangleData(std::stringstream& ss, ShapeData& data)
{
	Point topLeft = ReadPoint(ss);
	double width = ReadDouble(ss);
	double height = ReadDouble(ss);

	data.bounds = { topLeft, width, height };
}

void ReadTriangleData(std::stringstream& ss, ShapeData& data)
{
	Point bottomLeft = ReadPoint(ss);
	Point topMiddle = ReadPoint(ss);
	Point bottomRight = ReadPoint(ss);

	double width = bottomRight.x - bottomLeft.x;
	double height = topMiddle.y - bottomLeft.y;
	Point topLeft = { bottomLeft.x, bottomLeft.y + height };

	data.bounds = { topLeft, width, height };
}

void ReadTextData(std::stringstream& ss, ShapeData& data)
{
	Point topLeft = ReadPoint(ss);
	double fontSize = ReadDouble(ss);
	std::string text = ss.str();

	data.bounds = { topLeft, 0, 0 };
	data.fontSize = fontSize;
	data.text = std::move(text);
}

std::string ReadStringStrictly(std::stringstream& ss)
{
	std::string str;

	if (!(ss >> str))
	{
		throw CommandParseError("String shoud be non-empty");
	}

	return str;
}

Point ReadPoint(std::stringstream& ss)
{
	double x;
	double y;

	if (!(ss >> x) || !(ss >> y))
	{
		throw CommandParseError("Invalid point coordinates");
	}

	return { x, y };
}

Color ReadColor(std::stringstream& ss)
{
	Color color;
	if (!(ss >> std::hex >> color))
	{
		throw CommandParseError("Invalid color format. It should be six-digit hex number");
	}
	ss >> std::dec;

	return color;
}

double ReadDouble(std::stringstream& ss)
{
	double num;
	if (!(ss >> num))
	{
		throw CommandParseError("Invalid number encountered. It should be double divided with point");
	}

	return num;
}