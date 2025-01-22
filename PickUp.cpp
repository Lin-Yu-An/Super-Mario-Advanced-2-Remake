#include "pch.h"
#include "PickUp.h"
#include <iostream>
#include "Sprite.h"
#include "utils.h"

PickUp::PickUp(const Rectf& rect)
{
	m_Shape = rect;
	m_RectVertices = m_Shape.RectfToVectorPoints();
}

PickUp::PickUp(const Point2f& pos, const std::string& path, int cols, int rows, float frameSec)
	: m_Pos{ pos }
	, m_Path{ path }
	, m_Cols{ cols }
	, m_Rows{ rows }
	, m_FrameSec{ frameSec }
	, m_pSprite{ nullptr }
{
	m_pSprite = new Sprite{ m_Path, m_Cols, m_Rows, m_FrameSec };
	m_Shape = Rectf{ m_Pos.x, m_Pos.y, m_pSprite->GetFrameWidth(), m_pSprite->GetFrameHeight() };
	m_RectVertices = m_Shape.RectfToVectorPoints();
}

PickUp::~PickUp()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

void PickUp::Update(float elapsedSec)
{
	if(m_pSprite != nullptr) m_pSprite->Update(elapsedSec);
}

bool PickUp::OverlappingCheck(const Rectf& avatarShape)
{
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		if (utils::IsOverlapping(m_Shape, avatarShape))
		{
			std::cout << "coin get" << '\n';
			delete m_pSprite;
			m_pSprite = nullptr;
			return true;
		}
	}
	return false;
}


