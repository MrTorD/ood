#pragma once

#include "IObserver.h"
#include <iostream>

class PictureObserver : public IObserver<Picture>
{
public:
	PictureObserver(Picture* picture)
		: m_subscribtion(picture->Subscribe(this))
	{
		m_picture = picture;
	}

	void Update() override
	{
		std::cout << std::format("Picture updated. Shapes: {}\n", m_picture->GetShapesCount());
	}

private:
	Subscribtion<Picture> m_subscribtion;
	Picture* m_picture;
};
