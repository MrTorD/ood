#include "AddShapeCommand.h"
#include "DeleteShapesCommand.h"
#include "Ellipse.h"
#include "PictureDocument.h"
#include "Rectangle.h"
#include "ShiftShapesCommand.h"
#include "Triangle.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

class ShapeComparer : public IShapeVisitor
{
public:
	ShapeComparer(const Shape& shape)
		: m_shape(shape)
	{
	}

	void Visit(const Rectangle& rect) override
	{
		auto shapePtr = dynamic_cast<const Rectangle*>(&m_shape);
		ASSERT_NE(shapePtr, nullptr);

		AssertShapeProperties(*shapePtr, rect);
	}

	void Visit(const Ellipse& el) override
	{
		auto shapePtr = dynamic_cast<const Ellipse*>(&m_shape);

		ASSERT_NE(shapePtr, nullptr);
		AssertShapeProperties(*shapePtr, el);
	}

	void Visit(const Triangle& tr) override
	{
		auto shapePtr = dynamic_cast<const Triangle*>(&m_shape);

		ASSERT_NE(shapePtr, nullptr);
		AssertShapeProperties(*shapePtr, tr);
	}

private:
	const Shape& m_shape;

	void AssertShapeProperties(const Shape& shape1, const Shape& shape2)
	{
		ASSERT_EQ(shape1.GetBounds(), shape2.GetBounds());
		ASSERT_EQ(shape1.GetColor(), shape2.GetColor());
	}
};

class DocumentFixture : public ::testing::Test
{
public:
	PictureDocument document;

	void SetUp() override
	{
		document.ExecuteCommand(
			std::make_unique<AddShapeCommand>(
				document.GetPicture(),
				std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF),
				0));

		document.ExecuteCommand(
			std::make_unique<AddShapeCommand>(
				document.GetPicture(),
				std::make_unique<Ellipse>(Point(320, 220), 60, 30, 0xFFDE21FF),
				1));

		document.ExecuteCommand(
			std::make_unique<AddShapeCommand>(
				document.GetPicture(),
				std::make_unique<Triangle>(Point(300, 250), 60, 60, 0xF9AAC0FF),
				2));
	}

	void AssertPicture(const Picture& pic1, const Picture& pic2)
	{
		ASSERT_EQ(pic1.GetShapesCount(), pic2.GetShapesCount());

		for (int i = 0; i < pic1.GetShapesCount(); i++)
		{
			AssertShape(pic1.GetShape(i), pic2.GetShape(i));
		}
	}

	Picture CopyPicture(Picture* picture)
	{
		Picture pic;
		for (int i = 0; i < picture->GetShapesCount(); i++)
		{
			auto shape = picture->GetShape(i);
			pic.AddShape(shape->Clone());
		}

		return pic;
	}

private:
	void AssertShape(const Shape& shape1, const Shape& shape2)
	{
		ShapeComparer comparer(shape2);

		shape1.Accept(comparer);
	}
};

TEST_F(DocumentFixture, SeveralUndo)
{
	document.Undo();
	document.Undo();

	Picture expected;
	expected.AddShape(std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF));

	AssertPicture(*document.GetPicture(), expected);
}

TEST_F(DocumentFixture, SeveralRedo)
{
	document.Undo();
	document.Undo();
	document.Redo();
	document.Redo();

	Picture expected;
	expected.AddShape(std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF));
	expected.AddShape(std::make_unique<Ellipse>(Point(320, 220), 60, 30, 0xFFDE21FF));
	expected.AddShape(std::make_unique<Triangle>(Point(300, 250), 60, 60, 0xF9AAC0FF));

	AssertPicture(*document.GetPicture(), expected);
}

TEST_F(DocumentFixture, RedoDoesNothingAfterAction)
{
	document.Undo();
	document.Undo();
	document.Undo();

	document.ExecuteCommand(
		std::make_unique<AddShapeCommand>(
			document.GetPicture(),
			std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF),
			2));

	document.Redo();
	document.Redo();

	Picture expected;
	expected.AddShape(std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF));

	AssertPicture(*document.GetPicture(), expected);
}

TEST_F(DocumentFixture, AddShapeUndoRedo)
{
	PictureDocument doc;
	doc.ExecuteCommand(
		std::make_unique<AddShapeCommand>(
			doc.GetPicture(),
			std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF),
			0));

	Picture expected;
	expected.AddShape(std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF));

	AssertPicture(*doc.GetPicture(), expected);

	doc.Undo();

	AssertPicture(*doc.GetPicture(), Picture());

	doc.Redo();

	AssertPicture(*doc.GetPicture(), expected);
}

TEST_F(DocumentFixture, DeleteShapeUndoRedo)
{
	PictureDocument doc;
	doc.ExecuteCommand(
		std::make_unique<AddShapeCommand>(
			doc.GetPicture(),
			std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF),
			0));
	doc.ExecuteCommand(
		std::make_unique<DeleteShapesCommand>(
			doc.GetPicture(),
			std::set<size_t>{ 0 }));

	AssertPicture(*doc.GetPicture(), Picture());

	doc.Undo();
	Picture expected;
	expected.AddShape(std::make_unique<Rectangle>(Point(150, 190), 200, 150, 0xB1FF21FF));

	doc.Redo();
	AssertPicture(*doc.GetPicture(), Picture());
}

TEST_F(DocumentFixture, ShiftShapeUndoRedo)
{
	Picture beforeAction = CopyPicture(document.GetPicture());

	document.ExecuteCommand(
		std::make_unique<ShiftShapesCommand>(
			document.GetPicture(),
			std::vector<size_t>{ 0, 2 },
			Bounds(30, 40, 200, 150)));

	Picture afterAction = CopyPicture(&beforeAction);
	afterAction.GetShape(0)->ShiftBy(Bounds(30, 40, 200, 150));
	afterAction.GetShape(2)->ShiftBy(Bounds(30, 40, 200, 150));

	AssertPicture(*document.GetPicture(), afterAction);

	document.Undo();
	AssertPicture(*document.GetPicture(), beforeAction);

	document.Redo();
	AssertPicture(*document.GetPicture(), afterAction);
}

TEST_F(DocumentFixture, RedoOnHistoryBorder)
{
	auto pic = CopyPicture(document.GetPicture());
	document.Redo();

	AssertPicture(pic, *document.GetPicture());
}

TEST_F(DocumentFixture, UndoOnHistoryBorder)
{
	PictureDocument doc;
	auto pic = CopyPicture(doc.GetPicture());

	doc.Undo();

	AssertPicture(pic, *doc.GetPicture());
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}