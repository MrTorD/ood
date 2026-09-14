#include "src/Canvas/SvgCanvas.h"
#include "src/Picture.h"
#include "src/PictureCommands/ParsingUtils.h"
#include "src/Shapes/Point.h"
#include "src/Shapes/Rect.h"
#include "src/Shapes/Shape.h"
#include "src/Shapes/ShapeGeometries/CircleGeometry.h"
#include "src/Shapes/ShapeGeometries/LineGeometry.h"
#include "src/Shapes/ShapeGeometries/RectangleGeometry.h"
#include "src/Shapes/ShapeGeometries/TextGeometry.h"
#include <iostream>
#include <memory>

int main()
{
	auto canvas = std::make_unique<SvgCanvas>("drawing.svg");
	Picture picture(std::move(canvas));

	std::string str;

	try
	{
		while (std::getline(std::cin, str))
		{
			std::stringstream ss(str);
			auto command = ReadCommand(ss);
			command->Perform(picture);
		}
	}
	catch (const CommandParseError& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const InvalidShapeError& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const NotFoundError& e)
	{
		std::cout << e.what() << '\n';
	}
}
