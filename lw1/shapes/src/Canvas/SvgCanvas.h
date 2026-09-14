#pragma once

#include "ICanvas.h"
#include <format>
#include <fstream>
#include <sstream>

// validation??
class SvgCanvas : public ICanvas
{
public:
	SvgCanvas(const std::string& fileName)
	{
		m_fileName = fileName;
	}

	void DrawLine(Point from, Point to, Color color) override
	{
		m_drawing << std::format(R"(<line x1="{}" x2="{}" y1="{}" y2="{}" stroke="#{:06x}" />)",
			from.x, to.x, from.y, to.y, color);
			
		Flush();
	}

	void DrawPolygon(const std::vector<Point>& points, Color color) override
	{
		m_drawing << R"(<polygon points=")";

		for (const auto& point : points)
		{
			m_drawing << std::format(R"( {},{} )", point.x, point.y);
		}

		m_drawing << std::format(R"(" fill="#{:06x}" />)", color);

		Flush();
	}

	void DrawEllipse(Point centre, Point radiuses, Color color) override
	{
		m_drawing << std::format(R"(<ellipse cx="{}" cy="{}" rx="{}" ry="{}" fill="#{:06x}" />)",
			centre.x, centre.y, radiuses.x, radiuses.y, color);

		Flush();
	}

	void DrawText(Point topLeft, double fontSize, const std::string& text, Color color) override
	{
		m_drawing << std::format(R"(<text x="{}" y="{}" font-size="{}" fill="#{:06x}">{}</text>)",
			topLeft.x, topLeft.y, fontSize, color, text);

		Flush();
	}

	void Flush() override
	{
		std::ofstream output(m_fileName);

		output << "<svg xmlns=\"http://www.w3.org/2000/svg\">\n"
			   << m_drawing.str() << "\n"
			   << "</svg>\n";
	}

private:
	std::string m_fileName;
	std::stringstream m_drawing;
};
