#include "pch.h"
#include "Mushroom.h"
#include <iostream>
#include "Sprite.h"

Mushroom::Mushroom(Point2f pos)
	: PickUp(pos, "Resources/Images/mushroom.png", 1, 1, 1.f)
	, m_MushroomState{ MushroomState::yetSpawned }
	, m_IsHit{ false }
{
	
}

void Mushroom::Draw() const
{
	if (m_pSprite != nullptr) m_pSprite->Draw(m_Pos);
}

void Mushroom::Update(float elapsedSec)
{
	if (!m_IsHit && m_pSprite != nullptr)
	{
		m_pSprite->Update(elapsedSec);
	}

	if (m_MushroomState == MushroomState::spawning)
	{
		m_Shape.bottom += 2.f * elapsedSec;
	}
}

bool Mushroom::OverlappingCheck(const Rectf& avatarShape)
{
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		if (utils::IsOverlapping(m_Shape, avatarShape))
		{
			std::cout << "mushroom get" << '\n';
			delete m_pSprite;
			m_pSprite = nullptr;
			return true;
		}
	}
	return false;
}

Rectf Mushroom::GetMushShape() const
{
	return m_Shape;
}


