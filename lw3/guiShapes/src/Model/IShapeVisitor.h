#pragma once

class Rectangle;
class Ellipse;
class Triangle;
class GroupShape;

class IShapeVisitor
{
public:
	virtual void Visit(const Rectangle&) = 0;
	virtual void Visit(const Ellipse&) = 0;
	virtual void Visit(const Triangle&) = 0;
	virtual void Visit(const GroupShape&) = 0;

	virtual ~IShapeVisitor() = default;
};