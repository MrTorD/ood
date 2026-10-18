#pragma once

#include "IEditorShapeVisitor.h"
#include "IShapeVisitor.h"

class VisitorAdapter final : public IShapeVisitor
{
public:
	VisitorAdapter(const EditorShape& shape, IEditorShapeVisitor& visitor)
		: m_shape(shape)
		, m_visitor(visitor)
	{
	}

	void Visit(const Rectangle&) override
	{
		m_visitor.VisitRectangle(m_shape);
	}

	void Visit(const Ellipse&) override
	{
		m_visitor.VisitEllipse(m_shape);
	}

	void Visit(const Triangle&) override
	{
		m_visitor.VisitTriangle(m_shape);
	}

private:
	const EditorShape& m_shape;
	IEditorShapeVisitor& m_visitor;
};