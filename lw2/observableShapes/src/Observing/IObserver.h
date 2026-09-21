#pragma once

template <typename TSubject>
class IObserver
{
public:
	virtual void Update() = 0;

	virtual void CancelSubscribtion(TSubject* subject) = 0;

	virtual ~IObserver() = default;
};