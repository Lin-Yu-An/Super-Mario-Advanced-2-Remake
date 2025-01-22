#include "pch.h"
#include <iostream>
#include "Sprite.h"
#include "YoshiCoin.h"

YoshiCoin::YoshiCoin(Point2f pos)
	: PickUp(pos, "Resources/Images/YoshiCoin.png", 4, 1, 0.1f)
	, m_IsHit{ false }
{
}

void YoshiCoin::Draw() const
{
	if (m_pSprite != nullptr) m_pSprite->Draw(m_Pos);
}

void YoshiCoin::Update(float elapsedSec)
{
	if (!m_IsHit && m_pSprite != nullptr)
	{
		m_pSprite->Update(elapsedSec);
	}
}

bool YoshiCoin::OverlappingCheck(const Rectf& avatarShape)
{
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		if (utils::IsOverlapping(m_Shape, avatarShape))
		{
			//std::cout << "Yoshi Coin get" << '\n';
			delete m_pSprite;
			m_pSprite = nullptr;
			return true;
		}
	}
	return false;
}