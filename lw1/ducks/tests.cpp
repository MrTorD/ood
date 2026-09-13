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

	MOCK_METHOD(
		bool,
		IsFlyable,
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
	MockDuck() = default;

	MockDuck(std::unique_ptr<IFlyBehavior> flyBehavior,
		std::unique_ptr<IQuackBehavior> quackBehavior,
		std::unique_ptr<IDanceBehavior> danceBehavior)
		: Duck(std::move(flyBehavior),
			  std::move(quackBehavior),
			  std::move(danceBehavior))
	{
	}

	std::string GetName() const override
	{
		return "MockDuck";
	}
};

class DuckFixture : public ::testing::Test
{
public:
	void SetUp() override
	{
		flyBehavior = new MockFlyBehavior();
		quackBehavior = new MockQuackBehavior();
		danceBehavior = new MockDanceBehavior();

		duck = MockDuck(std::unique_ptr<MockFlyBehavior>(flyBehavior),
			std::unique_ptr<MockQuackBehavior>(quackBehavior),
			std::unique_ptr<MockDanceBehavior>(danceBehavior));
	}

	MockDuck duck;

	MockFlyBehavior* flyBehavior;
	MockQuackBehavior* quackBehavior;
	MockDanceBehavior* danceBehavior;
};

TEST_F(DuckFixture, FlyableDuckDanceBehavior)
{
	EXPECT_CALL(*danceBehavior, Dance())
		.Times(1);

	duck.Dance();
}

TEST_F(DuckFixture, FlyableDuckNoQuackAfterSingleFly)
{

	EXPECT_CALL(*flyBehavior, Fly())
		.Times(1);

	EXPECT_CALL(*flyBehavior, IsFlyable())
		.Times(1);

	ON_CALL(*flyBehavior, IsFlyable)
		.WillByDefault(::testing::Return(true));

	ON_CALL(*flyBehavior, Fly())
		.WillByDefault([&]() {
			flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*quackBehavior, Quack())
		.Times(0);

	duck.Fly();
}

TEST_F(DuckFixture, FlyableDuckSingleQuackAfterDoubleFly)
{
	EXPECT_CALL(*flyBehavior, Fly())
		.Times(2);

	EXPECT_CALL(*flyBehavior, IsFlyable())
		.Times(2);

	ON_CALL(*flyBehavior, IsFlyable)
		.WillByDefault(::testing::Return(true));

	ON_CALL(*flyBehavior, Fly())
		.WillByDefault([&]() {
			flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*quackBehavior, Quack())
		.Times(1);

	duck.Fly();
	duck.Fly();
}

TEST_F(DuckFixture, FlyableDuckHalfQuacksOfEvenFliesCount)
{
	EXPECT_CALL(*flyBehavior, Fly())
		.Times(4);

	EXPECT_CALL(*flyBehavior, IsFlyable())
		.Times(4);

	ON_CALL(*flyBehavior, IsFlyable)
		.WillByDefault(::testing::Return(true));

	ON_CALL(*flyBehavior, Fly())
		.WillByDefault([&]() {
			flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*quackBehavior, Quack())
		.Times(2);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST_F(DuckFixture, FlyableDuckHalfMinusOneQuacksOfOddFliesCount)
{
	EXPECT_CALL(*flyBehavior, Fly())
		.Times(5);

	EXPECT_CALL(*flyBehavior, IsFlyable())
		.Times(5);

	ON_CALL(*flyBehavior, IsFlyable)
		.WillByDefault(::testing::Return(true));

	ON_CALL(*flyBehavior, Fly())
		.WillByDefault([&]() {
			flyBehavior->m_fliesCount++;
		});

	EXPECT_CALL(*quackBehavior, Quack())
		.Times(2);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST_F(DuckFixture, UnflyableDuckZeroQuacksOnSingleFly)
{
	EXPECT_CALL(*flyBehavior, Fly())
		.Times(1);

	EXPECT_CALL(*flyBehavior, IsFlyable())
		.Times(1);

	ON_CALL(*flyBehavior, IsFlyable)
		.WillByDefault(::testing::Return(false));

	EXPECT_CALL(*quackBehavior, Quack())
		.Times(0);

	duck.Fly();
}

TEST_F(DuckFixture, UnflyableDuckZeroQuacksOnMultipleFlies)
{
	EXPECT_CALL(*flyBehavior, Fly())
		.Times(4);

	EXPECT_CALL(*flyBehavior, IsFlyable())
		.Times(4);

	EXPECT_CALL(*quackBehavior, Quack())
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