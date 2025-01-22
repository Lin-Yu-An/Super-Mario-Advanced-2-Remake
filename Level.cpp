#include "pch.h"
#include "Level.h"
#include <iostream>
#include "SVGParser.h"
#include "Platform.h"
#include "StaticPlatform.h"
#include "Coin.h"
#include "YoshiCoin.h"

Level::Level()
	: m_BoundariesVertices{Point2f{0,0}, Point2f{0,632}, Point2f{6400,632}, Point2f{6400, 0}}
{
	// [Background Sprites and SVG]
	SVGParser::GetVerticesFromSvgFile("Resources/Images/ForeGroundSVG.svg", m_Vertices);
	m_pBackgroundTexture = new Texture{ "Resources/Images/LevelBackground.png" };
	m_pForegroundTexture = new Texture{ "Resources/Images/LevelForeground03.png" };
	
	// [Platforms]
	InitPlatforms();
	
	// [PickUp Coins && PowerUps]
	InitPickUps();
}

Level::~Level()
{
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;

	delete m_pForegroundTexture;
	m_pForegroundTexture = nullptr;

	for (size_t i = 0; i < m_Platforms.size(); i++)
	{
		delete m_Platforms[i];
		m_Platforms[i] = nullptr;
	}

	for (size_t i = 0; i < m_PickUps.size(); i++)
	{
		delete m_PickUps[i];
		m_PickUps[i] = nullptr;
	}
}

void Level::InitPlatforms()
{
	m_Platforms.push_back(new StaticPlatform{ Rectf{79, 109, 96.f, 1.f } });
	m_Platforms.push_back(new StaticPlatform{ Rectf{48, 174, 78.f, 1.f } });
	m_Platforms.push_back(new StaticPlatform{ Rectf{80, 237, 79.f, 1.f } });
	m_Platforms.push_back(new StaticPlatform{ Rectf{128, 285, 94.f, 1.f } });
	m_Platforms.push_back(new StaticPlatform{ Rectf{272, 269, 63.f, 1.f } });

	m_Platforms.push_back(new QuestionBlock{ Point2f{200.f, 100.f} });
}

void Level::InitPickUps()
{
	m_PickUps.push_back(new Coin{ Point2f{230.f, 100.f } });
	m_PickUps.push_back(new Coin{ Point2f{250.f, 100.f } });
	m_PickUps.push_back(new Coin{ Point2f{270.f, 100.f } });

	m_PickUps.push_back(new Coin{ Point2f{100.f, 120.f } });
	m_PickUps.push_back(new Coin{ Point2f{120.f, 120.f } });
	m_PickUps.push_back(new Coin{ Point2f{140.f, 120.f } });

	m_PickUps.push_back(new Coin{ Point2f{60.f, 185.f } });
	m_PickUps.push_back(new Coin{ Point2f{80.f, 185.f } });
	m_PickUps.push_back(new Coin{ Point2f{100.f, 185.f } });

	m_PickUps.push_back(new YoshiCoin{ Point2f{300.f, 100.f} });
}

void Level::Update(float elapsedSec)
{
	for (Platform* p : m_Platforms)
	{
		p->Update(elapsedSec);
	}

	for (PickUp* c : m_PickUps)
	{
		c->Update(elapsedSec);
	}
}

void Level::DrawBackGround() const
{
	m_pBackgroundTexture->Draw();

	// [DEBUG]: DrawLevelCollision
	utils::SetColor(Color4f{ 0.0f,1.0f,0.0f,0.5f });
	for (const std::vector<Point2f>& v : m_Vertices)
		utils::DrawPolygon(v);
}

void Level::DrawForeGround() const
{
	m_pForegroundTexture->Draw();

	for (size_t i = 0; i < m_Platforms.size(); i++)
	{
		m_Platforms[i]->Draw();
	}

	for (size_t i = 0; i < m_PickUps.size(); i++)
	{
		m_PickUps[i]->Draw();
	}
}

void Level::HandleCollision(Rectf& avatarShape, Vector2f& avatarVelocity)
{
	//Vertical Raycast
	const Point2f verRayTopLeft{ avatarShape.left, avatarShape.bottom + avatarShape.height + 1 };
	const Point2f verRayBotLeft{ avatarShape.left, avatarShape.bottom - 1 };
	const Point2f verRayTopRight{ avatarShape.left + avatarShape.width, avatarShape.bottom + avatarShape.height + 1 };
	const Point2f verRayBotRight{ avatarShape.left + avatarShape.width, avatarShape.bottom - 1 };

	//Horizontal Raycast
	const Point2f horRayTopLeft{ avatarShape.left - 1, avatarShape.bottom + avatarShape.height };
	const Point2f horRayTopRight{ avatarShape.left + avatarShape.width + 1, avatarShape.bottom + avatarShape.height };
	const Point2f horRayBotLeft{ avatarShape.left - 1, avatarShape.bottom };
	const Point2f horRayBotRight{ avatarShape.left + avatarShape.width + 1, avatarShape.bottom };

	utils::HitInfo hitInfo{};
	for (size_t i{ 0 }; i < m_Vertices.size(); ++i)
	{		
		// [Top && Bottom Hitbox]
		if (utils::Raycast(m_Vertices[i], verRayTopLeft, verRayBotLeft, hitInfo) || utils::Raycast(m_Vertices[i], verRayTopRight, verRayBotRight, hitInfo))
		{
			// [Hitting ceiling or blocks on top of Mario]
			if (hitInfo.intersectPoint.y > avatarShape.bottom + (avatarShape.height / 2))
			{
				avatarShape.bottom = hitInfo.intersectPoint.y - avatarShape.height - 1;
				avatarVelocity.y = 0;
			}
			else // [Touching ground]
			{
				avatarShape.bottom = hitInfo.intersectPoint.y + 1;
				avatarVelocity.y = 0;
			}
		}
	}
	for (size_t i{ 0 }; i < m_Vertices.size(); ++i)
	{
		// [Left && Right Hitbox]
		if (utils::Raycast(m_Vertices[i], horRayTopLeft, horRayTopRight, hitInfo) || utils::Raycast(m_Vertices[i], horRayBotLeft, horRayBotRight, hitInfo))
		{
			if (hitInfo.intersectPoint.x > avatarShape.left + avatarShape.width / 2)
			{
				avatarShape.left = hitInfo.intersectPoint.x - avatarShape.width - 1;
				avatarVelocity.x = 0;
			}
			else
			{
				avatarShape.left = hitInfo.intersectPoint.x + 1;
				avatarVelocity.x = 0;
			}
		}
	}

	// [Platform collision]
	for (size_t i = 0; i < m_Platforms.size(); i++)
	{
		m_Platforms[i]->HandleCollision(avatarShape, avatarVelocity, *this);
	}

	for (size_t i = 0; i < m_PickUps.size(); i++)
	{
		m_PickUps[i]->OverlappingCheck(avatarShape);
	}
	
	//[Level boundaries collision]
	for (size_t i = 0; i < m_BoundariesVertices.size(); i++)
	{
		if (utils::Raycast(m_BoundariesVertices, horRayTopLeft, horRayTopRight, hitInfo) || utils::Raycast(m_Vertices[i], horRayBotLeft, horRayBotRight, hitInfo))
		{
			if (hitInfo.intersectPoint.x > avatarShape.left + avatarShape.width / 2)
			{
				avatarShape.left = hitInfo.intersectPoint.x - avatarShape.width - 1;
				avatarVelocity.x = 0;
			}
			else
			{
				avatarShape.left = hitInfo.intersectPoint.x + 1;
				avatarVelocity.x = 0;
			}
		}
	}
}

bool Level::IsOnGround(const Rectf& avatarShape, const Vector2f& avatarVelocity) 
{	
	//Vertical Raycast
	const Point2f verRayTopLeft{ avatarShape.left, avatarShape.bottom + avatarShape.height + 1 };
	const Point2f verRayBotLeft{ avatarShape.left, avatarShape.bottom - 1 };
	const Point2f verRayTopRight{ avatarShape.left + avatarShape.width, avatarShape.bottom + avatarShape.height + 1 };
	const Point2f verRayBotRight{ avatarShape.left + avatarShape.width, avatarShape.bottom - 1 };

	utils::HitInfo hitInfo{};
	for (size_t i{ 0 }; i < m_Vertices.size(); ++i)
	{
		// [Top && Bottom Hitbox]
		if (utils::Raycast(m_Vertices[i], verRayTopLeft, verRayBotLeft, hitInfo) || 
			utils::Raycast(m_Vertices[i], verRayTopRight, verRayBotRight, hitInfo))
		{
			return true;
		}
	}

	for (size_t i = 0; i < m_Platforms.size(); i++)
	{
		if (m_Platforms[i]->IsOnGround(avatarShape, avatarVelocity))
		{
			//std::cout << "stepping on platform" << '\n';
			return true;
		}
	}
	return false;
}

bool Level::IsHittingWall(const Rectf& avatarShape, const Vector2f& avatarVelocity)
{
	//Horizontal Raycast
	const Point2f horRayTopLeft{ avatarShape.left - 1, avatarShape.bottom + avatarShape.height };
	const Point2f horRayTopRight{ avatarShape.left + avatarShape.width + 1, avatarShape.bottom + avatarShape.height };
	const Point2f horRayBotLeft{ avatarShape.left - 1, avatarShape.bottom };
	const Point2f horRayBotRight{ avatarShape.left + avatarShape.width + 1, avatarShape.bottom };

	utils::HitInfo hitInfo{};
	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		//For each shape -> Raycast
		if (utils::Raycast(m_Vertices[i], horRayTopLeft, horRayTopRight, hitInfo) ||
		    utils::Raycast(m_Vertices[i], horRayBotLeft, horRayBotRight, hitInfo))
		{
			return true;
		}
	}
	return false;
}

void Level::AddPickUp(PickUp* pickup)
{
	m_PickUps.push_back(pickup);
}



