#include "Ellipse.h"
#include "GroupShapesCommand.h"
#include "PictureDocument.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "UngroupShapesCommand.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class DocumentFixture : public ::testing::Test
{
protected:
	PictureDocument document;
	Picture* picture = document.GetPicture();
	std::vector<size_t> indexes = { 0, 2 };
	IShape* rect;
	IShape* ellipse;
	IShape* triangle;

	void SetUp() override
	{
		picture->AddShape(std::make_unique<Rectangle>(Point(100, 100), 50, 50, Point(0, 0), 0));
		picture->AddShape(std::make_unique<Ellipse>(Point(100, 100), 50, 50, Point(0, 0), 0));
		picture->AddShape(std::make_unique<Triangle>(Point(100, 100), 50, 50, Point(0, 0), 0));

		rect = &picture->GetShape(0);
		ellipse = &picture->GetShape(1);
		triangle = &picture->GetShape(2);
	}
};

TEST_F(DocumentFixture, NonNeighboursGroupKeepsOrder)
{
	auto command = std::make_unique<GroupShapesCommand>(*picture, std::move(indexes));
	document.ExecuteCommand(std::move(command));

	ASSERT_EQ(ellipse, &picture->GetShape(0));
	auto* group = static_cast<GroupShape*>(&picture->GetShape(1));
	ASSERT_EQ(rect, &group->GetShape(0));
	ASSERT_EQ(triangle, &group->GetShape(1));
}

TEST_F(DocumentFixture, UndoGroupRestoreIndexes)
{
	auto command = std::make_unique<GroupShapesCommand>(*picture, std::move(indexes));
	document.ExecuteCommand(std::move(command));
	document.Undo();

	ASSERT_EQ(rect, &picture->GetShape(0));
	ASSERT_EQ(ellipse, &picture->GetShape(1));
	ASSERT_EQ(triangle, &picture->GetShape(2));
}

TEST_F(DocumentFixture, Ungroup)
{
	auto command = std::make_unique<GroupShapesCommand>(*picture, std::move(indexes));
	document.ExecuteCommand(std::move(command));
	auto Ungroupcommand = std::make_unique<UngroupShapesCommand>(*picture, 1);
	document.ExecuteCommand(std::move(Ungroupcommand));

	ASSERT_EQ(rect, &picture->GetShape(1));
	ASSERT_EQ(ellipse, &picture->GetShape(0));
	ASSERT_EQ(triangle, &picture->GetShape(2));
}

TEST_F(DocumentFixture, UngroupUndoRedo)
{
	auto command = std::make_unique<GroupShapesCommand>(*picture, std::move(indexes));
	document.ExecuteCommand(std::move(command));
	auto Ungroupcommand = std::make_unique<UngroupShapesCommand>(*picture, 1);
	document.ExecuteCommand(std::move(Ungroupcommand));
	document.Undo();

	ASSERT_EQ(ellipse, &picture->GetShape(0));
	auto* group = static_cast<GroupShape*>(&picture->GetShape(1));
	ASSERT_EQ(rect, &group->GetShape(0));
	ASSERT_EQ(triangle, &group->GetShape(1));

	document.Redo();

	ASSERT_EQ(rect, &picture->GetShape(1));
	ASSERT_EQ(ellipse, &picture->GetShape(0));
	ASSERT_EQ(triangle, &picture->GetShape(2));
}

TEST_F(DocumentFixture, ShiftGroup)
{
	auto command = std::make_unique<GroupShapesCommand>(*picture, std::move(indexes));
	document.ExecuteCommand(std::move(command));
	auto* group = static_cast<GroupShape*>(&picture->GetShape(1));
	group->ShiftBy({ 100, 100, 0, 0 });

	ASSERT_EQ(rect->GetBounds(), Bounds(200, 200, 50, 50));
	ASSERT_EQ(ellipse->GetBounds(), Bounds(50, 50, 100, 100));
	ASSERT_EQ(triangle->GetBounds(), Bounds(200, 200, 50, 50));
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}