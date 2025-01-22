#pragma once
#include "PickUp.h"

class YoshiCoin final : public PickUp
{
public:
	explicit YoshiCoin(Point2f pos);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;

	virtual bool OverlappingCheck(const Rectf& avatarShape) override;
private:
	bool m_IsHit;
};

