#include "pch.h"
#include "QuestionBlock.h"
#include "Texture.h"
#include "Sprite.h"
#include "Level.h"
#include "Mushroom.h"

QuestionBlock::QuestionBlock(Point2f pos)
	: Platform(pos, "Resources/Images/QuestionBlock.png", 5, 1, 0.2f)
	, m_BlockState{ BlockState::beforeHit }
	, m_IsHit{ false }
	, m_IsSpawned{ false }
{
}

void QuestionBlock::Draw() const
{
	m_pSprite->Draw(m_Pos);
	
	// [DEBUG]
	for (size_t i = 0; i < m_RectVertices.size(); i++)
	{
		utils::SetColor(Color4f(1, 0, 0, 1));
		utils::DrawRect(m_Shape);
	}
}

void QuestionBlock::Update(float elapsedSec)
{
	if (!m_IsHit)
	{
		if (m_pSprite->GetCurrentFrame() == 4) m_pSprite->SkipFrames(1);
		
		m_pSprite->Update(elapsedSec);
	}
}

void QuestionBlock::HandleCollision(Rectf& avatarShape, Vector2f& avatarVelocity, Level& level)
{
	const int offset{ 3 };
	
	//Vertical Raycast
	const Point2f verRayTopLeft{ avatarShape.left, avatarShape.bottom + avatarShape.height + offset };
	const Point2f verRayBotLeft{ avatarShape.left, avatarShape.bottom - offset };
	const Point2f verRayTopRight{ avatarShape.left + avatarShape.width, avatarShape.bottom + avatarShape.height + offset };
	const Point2f verRayBotRight{ avatarShape.left + avatarShape.width, avatarShape.bottom - offset };

	//Horizontal Raycast
	const Point2f horRayTopLeft{ avatarShape.left - offset, avatarShape.bottom + avatarShape.height };
	const Point2f horRayTopRight{ avatarShape.left + avatarShape.width + offset, avatarShape.bottom + avatarShape.height };
	const Point2f horRayBotLeft{ avatarShape.left - offset, avatarShape.bottom };
	const Point2f horRayBotRight{ avatarShape.left + avatarShape.width + offset, avatarShape.bottom };

	utils::HitInfo hitInfo{};
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		// [Top && Bottom Hitbox]
		if (utils::Raycast(m_RectVertices, verRayTopLeft, verRayBotLeft, hitInfo) || 
			utils::Raycast(m_RectVertices, verRayTopRight, verRayBotRight, hitInfo))
		{
			// [Hitting ceiling or blocks on top of Mario]
			if (hitInfo.intersectPoint.y > avatarShape.bottom + (avatarShape.height))
			{
				avatarShape.bottom = hitInfo.intersectPoint.y - avatarShape.height - offset;
				avatarVelocity.y = 0;
				m_pSprite->SetCurrentFrame(4);
				m_IsHit = true;
				
				if (m_BlockState == BlockState::beforeHit)
				{
					SpawnMushroom(level);
				}
			}
			else // [Touching ground]
			{
				avatarShape.bottom = hitInfo.intersectPoint.y + offset;
				avatarVelocity.y = 0;
			}
		}
	}
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		// [Left && Right Hitbox]
		if (utils::Raycast(m_RectVertices, horRayTopLeft, horRayTopRight, hitInfo) || 
			utils::Raycast(m_RectVertices, horRayBotLeft, horRayBotRight, hitInfo))
		{
			if (hitInfo.intersectPoint.x > avatarShape.left + avatarShape.width / 2)
			{
				avatarShape.left = hitInfo.intersectPoint.x - avatarShape.width - offset;
				avatarVelocity.x = 0;
			}
			else
			{
				avatarShape.left = hitInfo.intersectPoint.x + offset;
				avatarVelocity.x = 0;
			}
		}
	}
}

void QuestionBlock::SpawnMushroom(Level& level)
{
	level.AddPickUp(new Mushroom{ Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height } });
	m_BlockState = BlockState::isHit;
}

bool QuestionBlock::IsHit()
{
	return m_IsHit;
}
