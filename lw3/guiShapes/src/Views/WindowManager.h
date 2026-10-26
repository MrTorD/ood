#pragma once

#include "EditorWindow.h"
#include "Picture.h"
#include "PictureDocument.h"
#include <list>

class WindowManager
{
public:
	void AddWindow()
	{
		auto& document = m_documents.emplace_back();
		EmplaceWindow(document);
	}

	void Run()
	{
		while (!m_windows.empty())
		{
			for (auto& window : m_windows)
			{
				window.Render();
			}

			m_windows.remove_if(
				[this](EditorWindow& window) {
					return m_toDeleteWindows.contains(&window);
				});

			m_toDeleteWindows.clear();
		}
	}

private:
	// TODO: Обработка удаления документа если никто за ним не смотрит
	std::list<PictureDocument> m_documents;
	std::list<EditorWindow> m_windows;
	std::set<EditorWindow*> m_toDeleteWindows;

	void RemoveWindow(EditorWindow* window1)
	{
		m_windows.remove_if([&window1](const EditorWindow& window2) { return window1 == &window2; });
	}

	void AddWindow(std::unique_ptr<Picture> picture)
	{
		auto& document = m_documents.emplace_back(std::move(picture));
		EmplaceWindow(document);
	}

	void EmplaceWindow(PictureDocument& document)
	{
		m_windows.emplace_back(
			document,
			[this](EditorWindow* window) {
				m_toDeleteWindows.insert(window);
			},
			[this](std::unique_ptr<Picture> pic) { AddWindow(std::move(pic)); },
			[this](PictureDocument& doc) { EmplaceWindow(doc); });
	}
};