#include "Duck.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::MockFunction;

using FlyBehavior = MockFunction<std::function<int()>>;
using QuackBehavior = MockFunction<std::function<void()>>;
using DanceBehavior = MockFunction<std::function<void()>>;

struct MockDuck : Duck
{
	MockDuck(std::function<int()> flyBehavior,
		std::function<void()> quackBehavior,
		std::function<void()> danceBehavior)
		: Duck(flyBehavior,
			  quackBehavior,
			  danceBehavior)
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
	DanceBehavior danceBehavior;
	QuackBehavior quackBehavior;
	FlyBehavior flyBehavior;

	MockDuck duck = { flyBehavior.AsStdFunction(), quackBehavior.AsStdFunction(), danceBehavior.AsStdFunction() };
};

TEST_F(DuckFixture, ValidDanceBehavior)
{
	EXPECT_CALL(danceBehavior, Call())
		.Times(1);

	duck.Dance();
}

TEST_F(DuckFixture, NoQuackAfterSingleFly)
{
	EXPECT_CALL(flyBehavior, Call())
		.Times(1);

	EXPECT_CALL(quackBehavior, Call())
		.Times(0);

	duck.Fly();
}

TEST_F(DuckFixture, FlyableDuckSingleQuackAfterDoubleFly)
{
	unsigned fliesCount = 0;

	EXPECT_CALL(flyBehavior, Call())
		.Times(2);

	ON_CALL(flyBehavior, Call())
		.WillByDefault([&fliesCount]() mutable {
			return ++fliesCount;
		});

	EXPECT_CALL(quackBehavior, Call())
		.Times(1);

	duck.Fly();
	duck.Fly();
}

TEST_F(DuckFixture, FlyableDuckHalfQuacksOfEvenFliesCount)
{
	unsigned fliesCount = 0;

	EXPECT_CALL(flyBehavior, Call())
		.Times(4);

	ON_CALL(flyBehavior, Call())
		.WillByDefault([&fliesCount]() mutable {
			return ++fliesCount;
		});

	EXPECT_CALL(quackBehavior, Call())
		.Times(2);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST_F(DuckFixture, HalfMinusOneQuacksOfOddFliesCount)
{
	unsigned fliesCount = 0;
	EXPECT_CALL(flyBehavior, Call())
		.Times(5);

	ON_CALL(flyBehavior, Call())
		.WillByDefault([&fliesCount]() mutable {
			return ++fliesCount;
		});

	EXPECT_CALL(quackBehavior, Call())
		.Times(2);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST_F(DuckFixture, UnflyableDuckZeroQuacksOnSingleFly)
{
	EXPECT_CALL(flyBehavior, Call())
		.Times(1);

	EXPECT_CALL(quackBehavior, Call())
		.Times(0);

	duck.Fly();
}

TEST_F(DuckFixture, UnflyableDuckZeroQuacksOnMultipleFlies)
{
	EXPECT_CALL(flyBehavior, Call())
		.Times(4);

	EXPECT_CALL(quackBehavior, Call())
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