#pragma once
#include "utils.h"
#include <vector>

class Level;
class Sprite;

class Platform
{
public:
	explicit Platform(const Rectf& rect);
	Platform(const Point2f& pos, const std::string& path, int cols, int rows, float frameSec = 0);
	virtual ~Platform();
	Platform(const Platform& other) = delete;
	Platform& operator=(const Platform& other) = delete;
	Platform(Platform&& other) = delete;
	Platform& operator=(Platform&& other) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec);
	
	virtual void HandleCollision(Rectf& avatarShape, Vector2f& avatarVelocity, Level& level);
	bool IsOnGround(const Rectf& avatarShape, const Vector2f& avatarVelocity);

protected:
	Point2f m_Pos;
	std::string m_Path;
	int m_Cols;
	int m_Rows;
	float m_FrameSec;
	
	Rectf m_Shape;
	std::vector<Point2f> m_RectVertices;
	
	Sprite* m_pSprite;
	bool m_BGPlatform;
private:
	
};
