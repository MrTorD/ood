#pragma once

#include <memory>

template <typename TClone, typename TBase>
class PrototypeImpl : public TBase
{
public:
	std::unique_ptr<TBase> Clone() const override
	{
		return std::make_unique<TClone>(*static_cast<const TClone*>(this));
	}
};
