#pragma once
#include "Platform.h"

class StaticPlatform final : public Platform
{
public:
	explicit StaticPlatform(const Rectf& rect);

	virtual void Draw() const override;
private:
};