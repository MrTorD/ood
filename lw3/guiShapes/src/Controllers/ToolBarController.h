#pragma once

#include "Button.h"
#include "EditorSession.h"
#include "Picture.h"
#include "PictureSerialiser.h"
#include "Rectangle.h"
#include <vector>

class ToolBarController
{
public:
	ToolBarController(Picture& picture, EditorSession& session)
		: m_picture(picture)
		, m_session(session)
	{
		// TODO: контроллер не должен знать про расположение кнопок
		m_buttons.emplace_back(
			"Add rectangle",
			[](Picture& picture) {
				picture.AddShape(std::make_unique<Rectangle>(Point(400, 390), 200, 150, 0xB1FF21FF));
			},
			Bounds(10, 10, 110, 40),
			0x00FFFFFF);
		m_buttons.emplace_back(
			"Add ellipse",
			[](Picture& picture) {
				picture.AddShape(std::make_unique<Ellipse>(Point(320, 220), 60, 30, 0xFFDE21FF));
			},
			Bounds(10, 60, 110, 40),
			0x00FFFFFF);
		m_buttons.emplace_back(
			"Add triangle",
			[](Picture& picture) {
				picture.AddShape(std::make_unique<Triangle>(Point(130, 130), Point(190, 190), Point(250, 145), 0xF9AAC0FF));
			},
			Bounds(10, 110, 110, 40),
			0x00FFFFFF);
		m_buttons.emplace_back(
			"Delete selected\nshape",
			[&](Picture& picture) {
				auto* selected = m_session.GetSelectedShape();
				picture.DeleteShape(selected);
				m_session.SetSelectedShape(nullptr);
			},
			Bounds(10, 160, 110, 40),
			0x00FFFFFF);
		m_buttons.emplace_back(
			"Save to\ndocument",
			[&](Picture& picture) {
				PictureSerialiser serialiser("picture.txt");
				serialiser.Serialise(picture);
			},
			Bounds(10, 210, 110, 40),
			0x00FFFFFF);

		m_buttons.emplace_back(
			"Load from\ndocument",
			[&](Picture& picture) {
				PictureSerialiser serialiser("picture.txt");
				picture = serialiser.Deserialise();
			},
			Bounds(10, 260, 110, 40),
			0x00FFFFFF);
	}

	void OnMousePressed(Point point)
	{
		auto it = std::find_if(
			m_buttons.begin(),
			m_buttons.end(),
			[&point](const Button& button) {
				return button.GetBounds().HitTest(point);
			});

		if (it != m_buttons.end())
		{
			it->Press(m_picture);
		}
	}

	size_t GetButtonsCount() const
	{
		return m_buttons.size();
	}

	const Button& GetButton(size_t index) const
	{
		return m_buttons.at(index);
	}

private:
	Picture& m_picture;
	EditorSession& m_session;
	std::vector<Button> m_buttons;
};