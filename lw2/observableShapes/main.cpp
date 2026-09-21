#include "Picture.h"
#include "PictureObserver.h"
#include "SvgCanvas.h"
#include <ParsingUtils.h>
#include <iostream>
#include <memory>

int main()
{
	auto canvas = std::make_unique<SvgCanvas>("drawing.svg");
	Picture picture(std::move(canvas));

	// auto subscribtion1 = picture.SubscribeToAddShape([](const Shape& shape) {
	// 	std::cout << "Added shape:\n";
	// 	shape.Print(std::cout);
	// 	std::cout << "\n";
	// });

	std::string str;

	try
	{
		while (std::getline(std::cin, str))
		{
			std::stringstream ss(str);
			// auto command = ReadCommand(ss);
			// command->Execute(picture);
		}
	}
	catch (const PictureInvalidOperation& e)
	{
		std::cout << e.what() << '\n';
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