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
		m_windows.emplace_back(
			document,
			[&](EditorWindow* window) { RemoveWindow(window); },
			[&](std::unique_ptr<Picture> picture) { AddWindow(std::move(picture)); });
	}

	// void AddWindow(PictureDocument& document)
	// {
	// 	m_windows.emplace_back(document);
	// }

	void Run()
	{
		while (!m_windows.empty())
		{
			auto it = m_windows.begin();
			int pos = 0;
			while (it != m_windows.end())
			{
				it->Run();
				it = GetIterator(pos++);
			}
			// for (int i = 0; i < m_windows.size(); i++)
			// {
			// 	std::cout << "i: " << i << "\n";
			// 	auto it = m_windows.begin();
			// 	std::advance(it, i);

			// 	if (it != m_windows.end())
			// 	{
			// 		it->Run();
			// 	}
			// }
		}
	}

private:
	std::list<PictureDocument> m_documents;
	std::list<EditorWindow> m_windows;

	void RemoveWindow(EditorWindow* window1)
	{
		m_windows.remove_if([&window1](const EditorWindow& window2) { return window1 == &window2; });
	}

	void AddWindow(std::unique_ptr<Picture> picture)
	{
		auto& document = m_documents.emplace_back(std::move(picture));
		m_windows.emplace_back(
			document,
			[&](EditorWindow* window) { RemoveWindow(window); },
			[&](std::unique_ptr<Picture> picture) { AddWindow(std::move(picture)); });
	}

	std::list<EditorWindow>::iterator GetIterator(int pos)
	{
		return std::next(m_windows.begin(), pos);
	}
};