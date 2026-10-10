#pragma once

#include "Ellipse.h"
#include "Picture.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <format>
#include <fstream>

class PictureSerialiser : IShapeOperation
{
public:
	PictureSerialiser(const std::string& fileName)
		: m_file(fileName)
	{
	}

	void Serialise(Picture& picture)
	{
		for (size_t i = 0; i < picture.GetShapesCount(); i++)
		{
			picture.GetShape(i).ApplyOperation(*this);
		}
	}

	Picture Deserialise()
	{
		Picture pic;

		return pic;
	}

private:
	std::fstream m_file;

	virtual void ApplyTo(const Rectangle& rect)
	{
		auto [x, y, w, h] = rect.GetBounds();
		m_file << std::format("rectangle {} {} {} {} {}\n", x, y, w, h, rect.GetColor());
	}

	virtual void ApplyTo(const Ellipse& el)
	{
		auto [x, y] = el.GetCentre();
		m_file << std::format("ellipse {} {} {} {} {}\n", x, y, el.GetRadiusX(), el.GetRadiusY(), el.GetColor());
	}

	virtual void ApplyTo(const Triangle& tr)
	{
		auto [x1, y1] = tr.GetVertex1();
		auto [x2, y2] = tr.GetVertex2();
		auto [x3, y3] = tr.GetVertex3();

		m_file << std::format("triangle {} {} {} {} {} {} {}\n", x1, y1, x2, y2, x3, y3, tr.GetColor());
	}
};