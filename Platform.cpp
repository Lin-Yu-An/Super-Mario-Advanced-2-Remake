#include "pch.h"
#include "Platform.h"
#include "Sprite.h"
#include "Level.h"

Platform::Platform(const Rectf& rect) : Platform(rect.GetBottomLeft(), std::string{}, 0, 0, 0)
{
	m_Shape = rect;
	m_RectVertices = m_Shape.RectfToVectorPoints();
}

Platform::Platform(const Point2f& pos, const std::string& path, int cols, int rows, float frameSec)
	: m_Pos{ pos }
	, m_Path{ path }
	, m_Cols{ cols }
	, m_Rows{ rows }
	, m_FrameSec{ frameSec }
	, m_RectVertices{}
	, m_pSprite{ nullptr }
	, m_BGPlatform{ false }
{
	if (m_Path == std::string{})
	{
		m_BGPlatform = true;
	}
	else
	{
		m_pSprite = new Sprite{ m_Path, m_Cols, m_Rows, m_FrameSec };
		m_Shape = Rectf{ m_Pos.x, m_Pos.y, m_pSprite->GetFrameWidth(), m_pSprite->GetFrameHeight() };
		m_RectVertices = m_Shape.RectfToVectorPoints();
	}
}

Platform::~Platform()
{
	//If not a background platform, there is a sprite that we need to delete
	if(!m_BGPlatform)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
}

void Platform::Update(float elapsedSec)
{
	if(!m_BGPlatform) m_pSprite->Update(elapsedSec);
}

void Platform::HandleCollision(Rectf& avatarShape, Vector2f& avatarVelocity, Level& level)
{
	//Collision with platforms depends on the direction the avatar moves,
	//when moving upwards it shouldn’t collide, however downwards it should.

	//Vertical Raycast
	const Point2f verRayTopLeft{ avatarShape.left, avatarShape.bottom + avatarShape.height};
	const Point2f verRayBotLeft{ avatarShape.left, avatarShape.bottom - 1 };
	const Point2f verRayTopRight{ avatarShape.left + avatarShape.width, avatarShape.bottom + avatarShape.height};
	const Point2f verRayBotRight{ avatarShape.left + avatarShape.width, avatarShape.bottom - 1 };

	utils::HitInfo hitInfo{};
	
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		// [Bottom Hitbox]
		if (utils::Raycast(m_RectVertices, verRayTopLeft, verRayBotLeft, hitInfo) ||
			utils::Raycast(m_RectVertices, verRayTopRight, verRayBotRight, hitInfo))
		{
			// [Touching ground]
			if (hitInfo.intersectPoint.y < avatarShape.bottom + (avatarShape.height / 2))
			{
				avatarShape.bottom = hitInfo.intersectPoint.y + 1;
				avatarVelocity.y = 0;
			}
		}
	}
}

bool Platform::IsOnGround(const Rectf& avatarShape, const Vector2f& avatarVelocity)
{
	//Vertical Raycast
	const Point2f verRayTopLeft{ avatarShape.left, avatarShape.bottom + avatarShape.height };
	const Point2f verRayBotLeft{ avatarShape.left, avatarShape.bottom - 1 };
	const Point2f verRayTopRight{ avatarShape.left + avatarShape.width, avatarShape.bottom + avatarShape.height };
	const Point2f verRayBotRight{ avatarShape.left + avatarShape.width, avatarShape.bottom - 1 };

	utils::HitInfo hitInfo{};
	for (size_t i{ 0 }; i < m_RectVertices.size(); ++i)
	{
		// [Top && Bottom Hitbox]
		if (utils::Raycast(m_RectVertices, verRayTopLeft, verRayBotLeft, hitInfo) ||
			utils::Raycast(m_RectVertices, verRayTopRight, verRayBotRight, hitInfo))
		{
			if (hitInfo.intersectPoint.y > avatarShape.bottom + avatarShape.height / 2)
			{
				return false;
			}
			else if (hitInfo.intersectPoint.y < avatarShape.bottom + avatarShape.height / 2)
			{
				return true;
			}
		}
	}
	return false;
}
