#include "BoostPicture.h"
#include "BoostShape.h"
#include "LineGeometry.h"
#include "SvgCanvas.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using Picture = BoostPicture;
using Shape = BoostShape;

class MockMethode
{
public:
	MOCK_METHOD(
		void,
		Update,
		(),
		());
};

class EventFixture : public ::testing::Test
{
public:
	Picture picture = { std::make_unique<SvgCanvas>("") };
	MockMethode m;
};

TEST_F(EventFixture, SubscribtionCauseUpdate)
{
	EXPECT_CALL(m, Update())
		.Times(1);

	auto sub = picture.OnShapeAdded([&](Shape& shape) {
		m.Update();
		ASSERT_EQ(shape.GetId(), "1");
	});

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(EventFixture, NoUpdateOnOtherEvents)
{
	EXPECT_CALL(m, Update())
		.Times(0);

	auto sub = picture.OnShapeAdded([&](Shape&) {
		m.Update();
	});

	picture.MovePicture(10, 20);
}

TEST_F(EventFixture, UpdateRecievesData)
{
	EXPECT_CALL(m, Update())
		.Times(1);

	auto sub = picture.OnShapeMoved([&](const std::string& id, double dx, double dy) {
		m.Update();
		ASSERT_EQ(dx, 10);
		ASSERT_EQ(dy, 20);
	});

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.MovePicture(10, 20);
}

TEST_F(EventFixture, OneObserverHasManySubs)
{
	EXPECT_CALL(m, Update())
		.Times(1);

	MockMethode m1;
	EXPECT_CALL(m1, Update())
		.Times(2);

	auto sub1 = picture.OnShapeMoved([&](const std::string& id, double dx, double dy) {
		m.Update();
	});

	auto sub2 = picture.OnShapeAdded([&](Shape& shape) {
		m1.Update();
		m1.Update();
	});

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.MovePicture(10, 20);
}

TEST_F(EventFixture, OneEventHasManyObservers)
{
	EXPECT_CALL(m, Update())
		.Times(2);

	auto sub1 = picture.OnShapeAdded([&](Shape& shape) {
		m.Update();
	});

	auto sub2 = picture.OnShapeAdded([&](Shape& shape) {
		m.Update();
	});

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(EventFixture, DifferentSubsTurnsOffIndependently)
{
	EXPECT_CALL(m, Update())
		.Times(1);

	MockMethode m1;

	EXPECT_CALL(m1, Update())
		.Times(3);

	auto sub1 = picture.OnShapeAdded([&](Shape&) {
		m.Update();
	});

	auto sub2 = picture.OnShapeMoved([&](const std::string& id, double dx, double dy) {
		m1.Update();
	});

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.MovePicture(10, 20);

	sub1.disconnect();
	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.MovePicture(10, 20);
}

TEST_F(EventFixture, RaiiCorrectlyUnsubscribes)
{
	EXPECT_CALL(m, Update())
		.Times(1);

	{
		auto sub1 = picture.OnShapeAdded([&](Shape&) {
			m.Update();
		});

		picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	}

	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(EventFixture, NewUpdateOnlyOnNextIteration)
{
	EXPECT_CALL(m, Update())
		.Times(1);

	auto sub1 = picture.OnShapeAdded([&](Shape&) {
		m.Update();
		auto sub2 = picture.OnShapeAdded([&](Shape&) {
			m.Update();
		});
	});

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(EventFixture, DirectShapeChangeCausePictureUpdate)
{
	EXPECT_CALL(m, Update())
		.Times(1);

	auto sub1 = picture.OnShapeMoved([&](const std::string& id, double dx, double dy) {
		m.Update();
	});

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	Shape& shape = picture.GetShape("1");

	shape.Move(10, 20);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}