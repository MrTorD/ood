#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ICanvas.h"

struct MockCanvas : ICanvas
{
	MOCK_METHOD(
		void,
		DrawLine,
		(Point from, Point to, Color color),
		(override));

	MOCK_METHOD(
		void,
		DrawPolygon,
		(const std::vector<Point>& points, Color color),
		(override));

	MOCK_METHOD(
		void,
		DrawEllipse,
		(Point centre, Point radiuses, Color color),
		(override));

	MOCK_METHOD(
		void,
		DrawText,
		(Point topLeft, double fontSize, const std::string& text, Color color),
		(override));
};

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}