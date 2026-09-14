#pragma once

#include "IShapeGeometry.h"

class TextGeometry : public IShapeGeometry
{
public:
	TextGeometry(const std::string& text, double fontSize)
	{
		if (fontSize < 0)
		{
			throw new InvalidShapeError("Text's font should be non-negative number");
		}

		m_text = text;
		m_fontSize = fontSize;
	}

	void Draw(ICanvas& canvas, Rect bounds, Color color) override
	{
		canvas.DrawText(bounds.GetTopLeft(), m_fontSize, m_text, color);
	}

	std::string GetName() const override
	{
		return "text";
	}

	void PrintParams(std::ostream& output, Rect bounds) const override
	{
		auto [x, y] = bounds.GetTopLeft();
		
		output << std::format("{} {} {} {}", x, y, m_fontSize, m_text);
	}

private:
	std::string m_text;
	double m_fontSize;
};
