#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height)
	:m_Width{ width }
	,m_Height{ height }
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target) const
{
	Point2f cameraPos{ Track(target) };
	Clamp(cameraPos);
	glTranslatef(-cameraPos.x, -cameraPos.y, 0);
	utils::DrawRect(cameraPos, m_Width, m_Height);
}

Point2f Camera::Track(const Rectf& target) const
{
	// -m_Width / 4 since the screen is scaled *2 in game.cpp
	return Point2f((target.left + target.width / 2) - (m_Width / 4), (target.bottom + target.height / 2) - (m_Height / 4));
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	bottomLeftPos.x = std::max(m_LevelBoundaries.left, std::min(m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width, bottomLeftPos.x));
	bottomLeftPos.y = std::max(m_LevelBoundaries.bottom, std::min(m_LevelBoundaries.bottom +m_LevelBoundaries.height - m_Height,bottomLeftPos.y ));
	//min, max, value
}
