#include "Bounds.h"
#include "CircleGeometry.h"
#include "ICanvas.h"
#include "LineGeometry.h"
#include "Picture.h"
#include "RectangleGeometry.h"
#include "Shape.h"
#include "TextGeometry.h"
#include "TriangleGeometry.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

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

class PictureFixture : public ::testing::Test
{
public:
	void SetUp() override
	{
		picture = new Picture(std::unique_ptr<MockCanvas>(canvas));

		picture->AddShape("triangle", std::make_unique<TriangleGeometry>(Bounds({ 10, 10 }, 10, 10)), 0xfff);
		picture->AddShape("circle", std::make_unique<CircleGeometry>(Bounds({ 10, 10 }, 10, 10)), 0xfff);
		picture->AddShape("rectangle", std::make_unique<RectangleGeometry>(Bounds({ 10, 10 }, 10, 10)), 0xfff);
		picture->AddShape("text", std::make_unique<TextGeometry>(Bounds({ 10, 10 }, 0, 0), "hello", 12), 0xfff);
		picture->AddShape("line", std::make_unique<LineGeometry>(Bounds({ 10, 10 }, 10, 10)), 0xfff);
	}

	void TearDown() override
	{
		delete picture;
	}

	MockCanvas* canvas = new MockCanvas();
	Picture* picture;
};

TEST_F(PictureFixture, AddShapeWithExistingId)
{
	ASSERT_THROW(picture->AddShape("triangle", std::make_unique<TriangleGeometry>(Bounds({ 10, 10 }, 10, 10)), 0xfff),
		PictureInvalidOperation);
}

TEST_F(PictureFixture, MoveExistingShape)
{
	picture->MoveShape("rectangle", 10, 10);

	auto& shape = picture->GetShape("rectangle");
	ASSERT_EQ(shape.GetBounds(), Bounds({ 20, 20 }, 10, 10));
}

TEST_F(PictureFixture, MoveShapeWithUnexistingId)
{
	ASSERT_THROW(picture->MoveShape("undefined", 10, 10),
		NotFoundError);
}

TEST_F(PictureFixture, DrawExistingShape)
{
	std::vector<Point> trianglePoints = { { 10, 0 }, { 15, 10 }, { 20, 0 } };

	EXPECT_CALL(*canvas, DrawPolygon(trianglePoints, 0xfff))
		.Times(1);

	picture->DrawShape("id_triangle");
}

TEST_F(PictureFixture, DrawShapeWithUnexistingId)
{
	ASSERT_THROW(picture->DrawShape("undefined"),
		NotFoundError);
}

TEST_F(PictureFixture, MovePicture)
{
	picture->MovePicture(10, 10);

	ASSERT_EQ(picture->GetShape("triangle").GetBounds(),
		Bounds({ 20, 20 }, 10, 10));

	ASSERT_EQ(picture->GetShape("circle").GetBounds(),
		Bounds({ 20, 20 }, 10, 10));

	ASSERT_EQ(picture->GetShape("rectangle").GetBounds(),
		Bounds({ 20, 20 }, 10, 10));

	ASSERT_EQ(picture->GetShape("text").GetBounds(),
		Bounds({ 20, 20 }, 0, 0));

	ASSERT_EQ(picture->GetShape("line").GetBounds(),
		Bounds({ 20, 20 }, 10, 10));
}

TEST_F(PictureFixture, DrawPicture)
{
	std::vector<Point> trianglePoints = { { 10, 0 }, { 15, 10 }, { 20, 0 } };
	std::vector<Point> rectanglePoints = { { 10, 10 }, { 20, 10 }, { 20, 0 }, { 10, 0 } };

	EXPECT_CALL(*canvas, DrawPolygon(trianglePoints, 0xfff))
		.Times(1);

	EXPECT_CALL(*canvas, DrawEllipse(Point(15, 5), Point(5, 5), 0xfff))
		.Times(1);

	EXPECT_CALL(*canvas, DrawPolygon(rectanglePoints, 0xfff))
		.Times(1);

	EXPECT_CALL(*canvas, DrawText(Point(10, 10), 12, "hello", 0xfff))
		.Times(1);

	EXPECT_CALL(*canvas, DrawLine(Point(10, 10), Point(20, 0), 0xfff))
		.Times(1);

	picture->DrawPicture();
}

TEST_F(PictureFixture, DeleteExistingShape)
{
	picture->DeleteShape("rectangle");

	ASSERT_THROW(picture->GetShape("rectangle"),
		NotFoundError);
}

TEST_F(PictureFixture, DeleteShapeWithUnexistingId)
{
	ASSERT_THROW(picture->DeleteShape("undefined"),
		NotFoundError);
}

TEST_F(PictureFixture, ClonedShapeExistUndependently)
{
	picture->CloneShape("rectangle", "newRectangle");

	picture->MoveShape("newRectangle", 10, 10);

	ASSERT_EQ(picture->GetShape("rectangle").GetBounds(),
		Bounds({ 10, 10 }, 10, 10));

	ASSERT_EQ(picture->GetShape("newRectangle").GetBounds(),
		Bounds({ 20, 20 }, 10, 10));
}

TEST_F(PictureFixture, CloneShapeWithUnexistingId)
{
	ASSERT_THROW(picture->CloneShape("undefined", "newRectangle"),
		NotFoundError);
}

TEST_F(PictureFixture, ClonedShapeHasAlreadyExistingId)
{
	ASSERT_THROW(picture->CloneShape("triangle", "rectangle"),
		PictureInvalidOperation);
}

TEST_F(PictureFixture, ChangeColorOnExistingShape)
{
	picture->ChangeShapeColor("rectangle", 0xaaa);

	auto& shape = picture->GetShape("rectangle");

	ASSERT_EQ(shape.GetColor(), 0xaaa);
}

TEST_F(PictureFixture, ChangeColorOnUnexistingShape)
{
	ASSERT_THROW(picture->ChangeShapeColor("undefined", 0xaaa),
		NotFoundError);
}

TEST_F(PictureFixture, ChangeShapeOnExistingShape)
{
	picture->ChangeShape("rectangle", std::make_unique<LineGeometry>(Bounds({ 0, 0 }, 1, 1)));

	auto& shape = picture->GetShape("rectangle");

	ASSERT_EQ(shape.GetBounds(), Bounds({ 0, 0 }, 1, 1));
}

TEST_F(PictureFixture, ChangeShapeOnUnexistingShape)
{
	ASSERT_THROW(picture->ChangeShape("undefined", std::make_unique<LineGeometry>(Bounds({ 0, 0 }, 1, 1))),
		NotFoundError);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}