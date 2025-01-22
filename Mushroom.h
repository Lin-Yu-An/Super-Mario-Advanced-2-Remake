#pragma once
#include "PickUp.h"

class Sprite;

class Mushroom final : public PickUp
{
public:
	enum class MushroomState
	{
		yetSpawned,
		spawning,
		moving
	};
	
	explicit Mushroom(Point2f pos);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;

	virtual bool OverlappingCheck(const Rectf& avatarShape) override;

	Rectf GetMushShape() const;

private:
	MushroomState m_MushroomState;
	bool m_IsHit;
};
