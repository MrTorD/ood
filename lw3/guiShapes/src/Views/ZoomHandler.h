#pragma once

#include <SFML/Graphics.hpp>

class ZoomHandler
{
public:
	using ScopedConnection = boost::signals2::scoped_connection;
	using ChangeSignal = boost::signals2::signal<void(int)>;

	explicit ZoomHandler(sf::View& view)
		: m_view(view)
		, m_zoomStep({ 0.25f * m_view.getSize().x, 0.25f * m_view.getSize().y })
		, m_initialSize(view.getSize())
	{
	}

	void Zoom()
	{
		if (m_zoomTimesApplied <= m_zoomLimit)
		{
			m_view.setSize(m_view.getSize() + m_zoomStep);
			m_zoomTimesApplied++;
			m_changeSignal(GetCurrentZoom());
		}
	}

	void Unzoom()
	{
		if (m_zoomTimesApplied > -m_zoomLimit)
		{
			m_view.setSize(m_view.getSize() - m_zoomStep);
			m_zoomTimesApplied--;
			m_changeSignal(GetCurrentZoom());
		}
	}

	void ResetZoom()
	{
		if (m_zoomTimesApplied != 0)
		{
			m_view.setSize(m_initialSize);

			m_zoomTimesApplied = 0;
			m_changeSignal(GetCurrentZoom());
		}
	}

	int GetCurrentZoom() const
	{
		return 100 * m_initialSize.x / m_view.getSize().x;
	}

	[[nodiscard]] ScopedConnection DoOnChange(const ChangeSignal::slot_type& slot)
	{
		return m_changeSignal.connect(slot);
	}

private:
	sf::View& m_view;
	sf::Vector2f m_zoomStep;
	sf::Vector2f m_initialSize;
	int m_zoomTimesApplied = 0;
	int m_zoomLimit = 3;

	ChangeSignal m_changeSignal;
};