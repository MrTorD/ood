#include "LineGeometry.h"
#include "Shape.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <CircleGeometry.h>
#include <Picture.h>
#include <SvgCanvas.h>
#include <memory>

template <typename TSubject>
class MockObserver : public IObserver<TSubject>
{
public:
	MockObserver(TSubject* subject)
		: m_subscribtion(subject->Subscribe(this))
	{
	}

	void Subscribe(TSubject* subject)
	{
		m_subscribtion = subject->Subscribe(this);
	}

	void CancelSubscribtion()
	{
		m_subscribtion.Cancel();
	}

	void CancelSubscribtion(TSubject* subject) override
	{
		m_subscribtion.Cancel();
	}

	MOCK_METHOD(
		void,
		Update,
		(),
		(override));

private:
	Subscribtion<TSubject> m_subscribtion;
};

class PictureObsFixture : public ::testing::Test
{
public:
	Picture picture = { std::make_unique<SvgCanvas>("") };
	MockObserver<Picture> observer = { &picture };
};

TEST(ShapeObserver, RecievesUpdate)
{
	Shape shape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	MockObserver<Shape> observer(&shape);

	EXPECT_CALL(observer, Update())
		.Times(2);

	shape.SetColor(0xaaa);
	shape.SetGeometry(std::make_unique<CircleGeometry>(Bounds({ 20, 20 }, 30, 40)));
}

TEST_F(PictureObsFixture, RecievesUpdateOnShapeUpdate)
{
	EXPECT_CALL(observer, Update)
		.Times(2);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.ChangeShapeColor("1", 0xccc);
}

TEST_F(PictureObsFixture, RecievesUpdateOnDirectShapeUpdate)
{
	EXPECT_CALL(observer, Update)
		.Times(3);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);

	auto& shape = picture.GetShape("1");

	shape.SetColor(0xaaa);
	shape.SetGeometry(std::make_unique<CircleGeometry>(Bounds({ 20, 20 }, 30, 40)));
}

TEST_F(PictureObsFixture, RecievesUpdateOnShapeAdd)
{
	EXPECT_CALL(observer, Update())
		.Times(2);

	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.AddShape("3", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, StartObservingOnAddedShape)
{
	EXPECT_CALL(observer, Update())
		.Times(2);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.MoveShape("1", 1, 1);
}

TEST_F(PictureObsFixture, StopsObservingAfterDeleting)
{
	EXPECT_CALL(observer, Update())
		.Times(2);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.DeleteShape("1");
	picture.MovePicture(10, 10);
	picture.MovePicture(10, 10);
}

TEST_F(PictureObsFixture, SeveralObserversRecievesUpdates)
{
	MockObserver<Picture> observer2(&picture);
	MockObserver<Picture> observer3(&picture);

	EXPECT_CALL(observer, Update())
		.Times(1);
	EXPECT_CALL(observer2, Update())
		.Times(1);
	EXPECT_CALL(observer3, Update())
		.Times(1);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, CancelSubStopUpdates)
{
	EXPECT_CALL(observer, Update())
		.Times(1);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);

	observer.CancelSubscribtion();
	picture.MovePicture(10, 10);
}

TEST_F(PictureObsFixture, MultipleSubsCauseSingleUpdate)
{
	auto sub = picture.Subscribe(&observer);
	auto sub1 = picture.Subscribe(&observer);
	auto sub2 = picture.Subscribe(&observer);

	EXPECT_CALL(observer, Update())
		.Times(2);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.MovePicture(10, 10);
}

TEST_F(PictureObsFixture, FailOperationDontCauseUpdate)
{
	EXPECT_CALL(observer, Update())
		.Times(0);

	try
	{
		picture.MoveShape("undefined", 3, 4);
	}
	catch (...)
	{
	}
}

TEST_F(PictureObsFixture, CancelSubscribtionSelfOnUpdate)
{
	ON_CALL(observer, Update())
		.WillByDefault([&]() {
			observer.CancelSubscribtion();
		});

	EXPECT_CALL(observer, Update())
		.Times(1);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);

	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, CancelSubscribtionNextOnUpdate)
{
	MockObserver<Picture> observer1(&picture);
	MockObserver<Picture> observer2(&picture);

	ON_CALL(observer, Update())
		.WillByDefault([&]() {
			observer1.CancelSubscribtion();
		});

	EXPECT_CALL(observer, Update())
		.Times(1);
	EXPECT_CALL(observer1, Update())
		.Times(0);
	EXPECT_CALL(observer2, Update())
		.Times(1);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, SubscribeNewObserverOnUpdate)
{
	MockObserver<Picture> observer1(&picture);
	MockObserver<Picture> observer2(&picture);
	observer2.CancelSubscribtion();

	ON_CALL(observer, Update())
		.WillByDefault([&]() {
			observer2.Subscribe(&picture);
		});

	EXPECT_CALL(observer, Update())
		.Times(2);
	EXPECT_CALL(observer1, Update())
		.Times(2);
	EXPECT_CALL(observer2, Update())
		.Times(1);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, SubsAndUnsubsOnUpdate)
{
	ON_CALL(observer, Update())
		.WillByDefault([&]() {
			observer.CancelSubscribtion();
			observer.Subscribe(&picture);
		});

	EXPECT_CALL(observer, Update())
		.Times(2);

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, SubscribtionDeathStopUpdates)
{
	EXPECT_CALL(observer, Update())
		.Times(1);

	observer.CancelSubscribtion();
	{
		auto sub = picture.Subscribe(&observer);
		picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	}

	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, SubjectDiesBeforeSub)
{
	EXPECT_CALL(observer, Update())
		.Times(1);

	observer.CancelSubscribtion();

	{
		Picture pic(std::make_unique<SvgCanvas>(""));
		observer.Subscribe(&pic);
		pic.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	}

	observer.CancelSubscribtion();
}

TEST_F(PictureObsFixture, SubscribtionsSupportsSeveralCancels)
{
	EXPECT_CALL(observer, Update())
		.Times(1);

	observer.CancelSubscribtion();
	auto sub = picture.Subscribe(&observer);
	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);

	sub.Cancel();
	sub.Cancel();

	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

TEST_F(PictureObsFixture, SubscribtionMovingGivesSubOwnership)
{
	EXPECT_CALL(observer, Update())
		.Times(2);

	observer.CancelSubscribtion();
	auto sub = picture.Subscribe(&observer);
	auto sub1 = std::move(sub);

	sub.Cancel();

	picture.AddShape("1", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
	picture.AddShape("2", std::make_unique<LineGeometry>(Bounds({ 10, 20 }, 30, 40)), 0xfff);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}