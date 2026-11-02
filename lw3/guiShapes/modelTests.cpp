#include "Ellipse.h"
#include "Picture.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class ModelFixture : public ::testing::Test
{
public:
	Picture picture;

	void SetUp() override
	{
		picture.AddShape(std::make_unique<Rectangle>(Point(100, 100), 50, 50, 0));
		picture.AddShape(std::make_unique<Ellipse>(Point(100, 100), 50, 50, 0));
		picture.AddShape(std::make_unique<Triangle>(Point(100, 100), 50, 50, 0));
	}

private:
};

TEST_F(ModelFixture, ShapeIsContains)
{
	Point p1(1000, 1000);
	Point p2(100, 100);
	Point p3(100, 125);
	Point p4(150, 150);

	auto* rect = picture.GetShape(0);
	auto* ellipse = picture.GetShape(1);
	auto* triangle = picture.GetShape(2);

	ASSERT_EQ(rect->IsContains(p1), false);
	ASSERT_EQ(rect->IsContains(p2), true);
	ASSERT_EQ(rect->IsContains(p3), true);
	ASSERT_EQ(rect->IsContains(p4), true);

	ASSERT_EQ(ellipse->IsContains(p1), false);
	ASSERT_EQ(ellipse->IsContains(p2), true);
	ASSERT_EQ(ellipse->IsContains(p3), true);
	ASSERT_EQ(ellipse->IsContains(p4), false);

	ASSERT_EQ(triangle->IsContains(p1), false);
	ASSERT_EQ(triangle->IsContains(p2), false);
	ASSERT_EQ(triangle->IsContains(p3), false);
	ASSERT_EQ(triangle->IsContains(p4), true);
}

TEST_F(ModelFixture, PictureLiftShape)
{
	auto* shape0 = picture.GetShape(0);
	auto* shape1 = picture.GetShape(1);
	auto* shape2 = picture.GetShape(2);
	picture.LiftShape(0, 2);

	ASSERT_EQ(shape0, picture.GetShape(2));
	ASSERT_EQ(shape1, picture.GetShape(0));
	ASSERT_EQ(shape2, picture.GetShape(1));
}

TEST_F(ModelFixture, InvalidGetShape)
{
	ASSERT_THROW(picture.GetShape(-1), std::out_of_range);
	ASSERT_THROW(picture.GetShape(4), std::out_of_range);
}

TEST_F(ModelFixture, ValidPictureGetIndex)
{
	auto* shape0 = picture.GetShape(0);
	auto* shape1 = picture.GetShape(1);
	auto* shape2 = picture.GetShape(2);

	ASSERT_EQ(0, picture.GetIndex(shape0));
	ASSERT_EQ(1, picture.GetIndex(shape1));
	ASSERT_EQ(2, picture.GetIndex(shape2));
}

TEST_F(ModelFixture, InvalidPictureGetIndex)
{
	auto shape = std::make_unique<Rectangle>(Point(0, 0), 10, 10, 0);

	ASSERT_EQ(-1, picture.GetIndex(shape.get()));
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}