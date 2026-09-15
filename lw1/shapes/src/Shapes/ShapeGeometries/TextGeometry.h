#pragma once

#include "IShapeGeometry.h"

class TextGeometry : public IShapeGeometry
{
public:
	TextGeometry(Point topLeft, const std::string& text, double fontSize)
	{
		if (fontSize < 0)
		{
			throw new InvalidShapeError("Text's font should be non-negative number");
		}

		m_topLeft = topLeft;
		m_text = text;
		m_fontSize = fontSize;
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.DrawText(bounds.GetTopLeft(), m_fontSize, m_text, color);
	}

	std::string GetName() const override
	{
		return "text";
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
