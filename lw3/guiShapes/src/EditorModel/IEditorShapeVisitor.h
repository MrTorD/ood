#pragma once

#include "GroupShape.h"

class EditorShape;

class IEditorShapeVisitor
{
public:
	virtual void VisitRectangle(const EditorShape&) = 0;
	virtual void VisitEllipse(const EditorShape&) = 0;
	virtual void VisitTriangle(const EditorShape&) = 0;
	virtual void VisitGroupShape(const EditorShape&, const GroupShape&) = 0;

protected:
	~IEditorShapeVisitor() = default;
};