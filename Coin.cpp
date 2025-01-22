#include "pch.h"
#include <iostream>
#include "Sprite.h"
#include "Coin.h"

Coin::Coin(Point2f pos)
	: PickUp(pos, "Resources/Images/Coin.png", 4, 1, 0.2f)
	, m_IsHit{ false }
{
}

void Coin::Draw() const
{
	if (m_pSprite != nullptr) m_pSprite->Draw(m_Pos);
}

void Coin::Update(float elapsedSec)
{
	if (!m_IsHit && m_pSprite != nullptr)
	{
		m_pSprite->Update(elapsedSec);
	}
}

bool Coin::OverlappingCheck(const Rectf& avatarShape)
{
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		if (utils::IsOverlapping(m_Shape, avatarShape))
		{
			//std::cout << "coin get" << '\n';
			delete m_pSprite;
			m_pSprite = nullptr;
			return true;
		}
	}
	return false;
}
