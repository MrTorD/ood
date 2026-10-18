#pragma once

class EditorShape;

class IEditorShapeVisitor
{
public:
	virtual void VisitRectangle(const EditorShape& shape) = 0;
	virtual void VisitEllipse(const EditorShape& shape) = 0;
	virtual void VisitTriangle(const EditorShape& shape) = 0;

protected:
	~IEditorShapeVisitor() = default;
};