#include "gmock/gmock-cardinalities.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Ducks/Duck.h"

struct MockFlyBehavior : IFlyBehavior
{
	unsigned GetFliesCount() override
	{
		return m_fliesCount;
	}

	MOCK_METHOD(
		void,
		Fly,
		(),
		(override));

	unsigned m_fliesCount = 0;
};

struct MockQuackBehavior : IQuackBehavior
{
	MOCK_METHOD(
		void,
		Quack,
		(),
		(override));
};

struct MockDanceBehavior : IDanceBehavior
{
	MOCK_METHOD(
		void,
		Dance,
		(),
		(override));
};

struct MockDuck : Duck
{
	MockDuck(MockFlyBehavior* flyBehavior,
		MockQuackBehavior* quackBehavior,
		MockDanceBehavior* danceBehavior)
		: Duck(std::unique_ptr<IFlyBehavior>(flyBehavior),
			  std::unique_ptr<IQuackBehavior>(quackBehavior),
			  std::unique_ptr<IDanceBehavior>(danceBehavior))
	{
		m_flyBehavior = flyBehavior;
		m_quackBehavior = quackBehavior;
		m_danceBehavior = danceBehavior;
	}

	std::string GetName() const override
	{
		return "MockDuck";
	}

	MockFlyBehavior* m_flyBehavior;
	MockQuackBehavior* m_quackBehavior;
	MockDanceBehavior* m_danceBehavior;
};

TEST(DanceBehavior, ValidDanceBehavior)
{
	MockDuck duck(new MockFlyBehavior(), new MockQuackBehavior(), new MockDanceBehavior());

	EXPECT_CALL(*duck.m_danceBehavior, Dance())
		.Times(1);

	duck.Dance();
}

TEST(FlyableDuckFlyBehavior, NoQuackAfterSingleFly)
{
	MockDuck duck(new MockFlyBehavior(), new MockQuackBehavior(), new MockDanceBehavior());

	EXPECT_CALL(*duck.m_flyBehavior, Fly())
		.Times(1);

	ON_CALL(*duck.m_flyBehavior, Fly())
		.WillByDefault([&duck]() {
			duck.m_flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*duck.m_quackBehavior, Quack())
		.Times(0);

	duck.Fly();
}

TEST(FlyableDuckFlyBehavior, SingleQuackAfterDoubleFly)
{
	MockDuck duck(new MockFlyBehavior(), new MockQuackBehavior(), new MockDanceBehavior());

	EXPECT_CALL(*duck.m_flyBehavior, Fly())
		.Times(2);

	ON_CALL(*duck.m_flyBehavior, Fly())
		.WillByDefault([&duck]() {
			duck.m_flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*duck.m_quackBehavior, Quack())
		.Times(1);

	duck.Fly();
	duck.Fly();
}

TEST(FlyableDuckFlyBehavior, HalfQuacksOfEvenFliesCount)
{
	MockDuck duck(new MockFlyBehavior(), new MockQuackBehavior(), new MockDanceBehavior());

	EXPECT_CALL(*duck.m_flyBehavior, Fly())
		.Times(4);

	ON_CALL(*duck.m_flyBehavior, Fly())
		.WillByDefault([&duck]() {
			duck.m_flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*duck.m_quackBehavior, Quack())
		.Times(2);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST(FlyableDuckFlyBehavior, HalfMinusOneQuacksOfOddFliesCount)
{
	MockDuck duck(new MockFlyBehavior(), new MockQuackBehavior(), new MockDanceBehavior());

	EXPECT_CALL(*duck.m_flyBehavior, Fly())
		.Times(5);

	ON_CALL(*duck.m_flyBehavior, Fly())
		.WillByDefault([&duck]() {
			duck.m_flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*duck.m_quackBehavior, Quack())
		.Times(2);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST(UnflyableDuckFlyBehavior, ZeroQuacksOnSingleFly)
{
	MockDuck duck(new MockFlyBehavior(), new MockQuackBehavior(), new MockDanceBehavior());

	EXPECT_CALL(*duck.m_flyBehavior, Fly())
		.Times(1);

	EXPECT_CALL(*duck.m_quackBehavior, Quack())
		.Times(0);

	duck.Fly();
}

TEST(UnflyableDuckFlyBehavior, ZeroQuacksOnMultipleFlies)
{
	MockDuck duck(new MockFlyBehavior(), new MockQuackBehavior(), new MockDanceBehavior());

	EXPECT_CALL(*duck.m_flyBehavior, Fly())
		.Times(4);

	EXPECT_CALL(*duck.m_quackBehavior, Quack())
		.Times(0);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}