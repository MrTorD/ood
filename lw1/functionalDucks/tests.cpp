#include "Ducks/Duck.h"

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

TEST(DanceBehavior, ValidDanceBehavior)
{
	DanceBehavior danceBehavior;
	QuackBehavior quackBehavior;
	FlyBehavior flyBehavior;

	MockDuck duck = MockDuck(flyBehavior.AsStdFunction(), quackBehavior.AsStdFunction(), danceBehavior.AsStdFunction());

	EXPECT_CALL(danceBehavior, Call())
		.Times(1);

	duck.Dance();
}

TEST(FlyableDuck, NoQuackAfterSingleFly)
{
	DanceBehavior danceBehavior;
	QuackBehavior quackBehavior;
	FlyBehavior flyBehavior;

	MockDuck duck = MockDuck(flyBehavior.AsStdFunction(), quackBehavior.AsStdFunction(), danceBehavior.AsStdFunction());

	EXPECT_CALL(flyBehavior, Call())
		.Times(1);

	EXPECT_CALL(quackBehavior, Call())
		.Times(0);

	duck.Fly();
}

// TEST_F(DuckFixture, FlyableDuckSingleQuackAfterDoubleFly)
// {
// 	unsigned fliesCount = 0;

// 	EXPECT_CALL(flyBehavior, Call())
// 		.Times(2)
// 		.WillRepeatedly(testing::Return(fliesCount));

// 	ON_CALL(flyBehavior, Call())
// 		.WillByDefault([&fliesCount]() mutable {
// 			return ++fliesCount;
// 		});

// 	EXPECT_CALL(quackBehavior, Call())
// 		.Times(1);

// 	duck.Fly();
// 	duck.Fly();
// }

// TEST_F(DuckFixture, FlyableDuckHalfQuacksOfEvenFliesCount)
// {

// 	EXPECT_CALL(flyBehavior, Call())
// 		.Times(4);

// 	// ON_CALL(*duck.m_flyBehavior, Fly())
// 	// 	.WillByDefault([&duck]() {
// 	// 		duck.m_flyBehavior->m_fliesCount++;
// 	// 	});

// 	EXPECT_CALL(quackBehavior, Call())
// 		.Times(2);

// 	duck.Fly();
// 	duck.Fly();
// 	duck.Fly();
// 	duck.Fly();
// }

// TEST_F(DuckFixture, HalfMinusOneQuacksOfOddFliesCount)
// {
// 	EXPECT_CALL(flyBehavior, Call())
// 		.Times(5);

// 	// ON_CALL(*duck.m_flyBehavior, Fly())
// 	// 	.WillByDefault([&duck]() {
// 	// 		duck.m_flyBehavior->m_fliesCount++;
// 	// 	});

// 	EXPECT_CALL(quackBehavior, Call())
// 		.Times(2);

// 	duck.Fly();
// 	duck.Fly();
// 	duck.Fly();
// 	duck.Fly();
// 	duck.Fly();
// }

// TEST_F(DuckFixture, UnflyableDuckZeroQuacksOnSingleFly)
// {
// 	EXPECT_CALL(flyBehavior, Call())
// 		.Times(1);

// 	EXPECT_CALL(quackBehavior, Call())
// 		.Times(0);

// 	duck.Fly();
// }

// TEST_F(DuckFixture, UnflyableDuckZeroQuacksOnMultipleFlies)
// {
// 	EXPECT_CALL(flyBehavior, Call())
// 		.Times(4);

// 	EXPECT_CALL(quackBehavior, Call())
// 		.Times(0);

// 	duck.Fly();
// 	duck.Fly();
// 	duck.Fly();
// 	duck.Fly();
// }

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}