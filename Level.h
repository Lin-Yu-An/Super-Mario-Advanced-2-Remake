#pragma once
#include <vector>
#include "Texture.h"
#include "utils.h"
#include "QuestionBlock.h"

class Sprite;
class Platform;
class PickUp;

class Level
{
public:
	Level();
	~Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;

	void InitPlatforms();
	void InitPickUps();
	
	void Update(float elapsedSec);
	void DrawBackGround() const;
	void DrawForeGround() const;

	void HandleCollision(Rectf& avatarShape, Vector2f& avatarVelocity);
	bool IsOnGround(const Rectf& avatarShape, const Vector2f& avatarVelocity);
	bool IsHittingWall(const Rectf& avatarShape, const Vector2f& avatarVelocity);

	void AddPickUp(PickUp* pickup);
	
private:
	std::vector<std::vector<Point2f>> m_Vertices;
	std::vector<Point2f> m_BoundariesVertices;
	
	Texture* m_pBackgroundTexture;
	Texture* m_pForegroundTexture;

	// [Platform]
	std::vector<Sprite*> m_PlatformSprites;
	std::vector<Platform*> m_Platforms;

	// [PickUp]
	std::vector<Sprite*> m_CoinSprites;
	std::vector<PickUp*> m_PickUps;
};