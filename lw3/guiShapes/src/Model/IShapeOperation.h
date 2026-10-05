#pragma once

class Rectangle;
class Ellipse;
class Triangle;

class IShapeOperation
{
public:
	virtual void ApplyTo(const Rectangle&) = 0;
	virtual void ApplyTo(const Ellipse&) = 0;
	virtual void ApplyTo(const Triangle&) = 0;

	virtual ~IShapeOperation() = default;
};