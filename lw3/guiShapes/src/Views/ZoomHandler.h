#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

class ZoomHandler
{
public:
	explicit ZoomHandler(sf::View& view)
		: m_view(view)
	{
	}

	void Zoom()
	{
		if (m_zoomTimesApplied < m_zoomTimesLimit)
		{
			m_view.zoom(m_zoomStep);
			m_zoomTimesApplied++;
		}
	}

	void Unzoom()
	{
		if (m_zoomTimesApplied > -m_zoomTimesLimit)
		{
			m_view.zoom(1 / m_zoomStep);
			m_zoomTimesApplied--;
		}
	}

	void ResetZoom()
	{
		m_view.zoom(std::powf(m_zoomStep, -m_zoomTimesApplied));

		m_zoomTimesApplied = 0;
	}

	float GetCurrentZoom() const
	{
		return 100 + m_zoomStep * m_zoomTimesApplied;
	}

private:
	sf::View& m_view;
	float m_zoomStep = 0.9;
	int m_zoomTimesApplied = 0;
	int m_zoomTimesLimit = 10;
};