#pragma once

#include "Picture.h"
#include <functional>

class Button
{
public:
	Button(const std::string& text, std::function<void(Picture&)> action, Bounds bounds, Color color)
		: m_text(text)
		, m_action(action)
		, m_bounds(bounds)
		, m_color(color)
	{
	}

	const std::string& GetText() const
	{
		return m_text;
	}

	Bounds GetBounds() const
	{
		return m_bounds;
	}

	Color GetColor() const
	{
		return m_color;
	}

	void Press(Picture& picture)
	{
		m_action(picture);
	}

private:
	std::string m_text;
	std::function<void(Picture&)> m_action;
	Bounds m_bounds;
	Color m_color;
};