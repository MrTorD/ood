#include "Bounds.h"
#include "CircleGeometry.h"
#include "LineGeometry.h"
#include "Picture.h"
#include "Point.h"
#include "RectangleGeometry.h"
#include "Shape.h"
#include "SvgCanvas.h"
#include "TextGeometry.h"
#include <iostream>
#include <memory>
#include "CommandParseError.h"
#include "ParsingUtils.h"

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
			command->Execute(picture);
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
