#pragma once

#include "IShapeGeometry.h"

class TextGeometry : public IShapeGeometry
{
public:
	TextGeometry(Bounds bounds, const std::string& text, double fontSize)
	{
		if (fontSize < 0)
		{
			throw new InvalidShapeError("Text's font should be non-negative number");
		}

		SetBounds(bounds);
		m_text = text;
		m_fontSize = fontSize;
	}

	std::string GetName() const override
	{
		return "text";
	}

	Bounds GetBounds() const override
	{
		return { m_topLeft, 0, 0 };
	}

	void SetBounds(Bounds bounds) override
	{
		m_topLeft = bounds.GetTopLeft();
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.DrawText(m_topLeft, m_fontSize, m_text, color);
	}

	void Move(double dx, double dy) override
	{
		m_topLeft = { m_topLeft.x + dx, m_topLeft.y + dy };
	}

	void PrintParams(std::ostream& output) const override
	{
		auto [x, y] = m_topLeft;

		output << std::format("{} {} {} {}", x, y, m_fontSize, m_text);
	}

private:
	Point m_topLeft;
	std::string m_text;
	double m_fontSize;
};
