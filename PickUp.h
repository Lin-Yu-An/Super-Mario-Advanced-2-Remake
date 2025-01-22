#pragma once
#include "utils.h"

class Sprite;

class PickUp
{
public:
	explicit PickUp(const Rectf& rect);
	PickUp(const Point2f& pos, const std::string& path, int cols, int rows, float frameSec = 0);
	virtual ~PickUp();
	PickUp(const PickUp& other) = delete;
	PickUp& operator=(const PickUp& other) = delete;
	PickUp(PickUp&& other) = delete;
	PickUp& operator=(PickUp&& other) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec);

	virtual bool OverlappingCheck(const Rectf& avatarShape);
protected:
	Point2f m_Pos;
	std::string m_Path;
	int m_Cols;
	int m_Rows;
	float m_FrameSec;
	Sprite* m_pSprite;

	Rectf m_Shape;
	std::vector<Point2f> m_RectVertices;
private:
	
};

