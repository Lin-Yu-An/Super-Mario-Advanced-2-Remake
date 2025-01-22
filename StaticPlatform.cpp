#include "pch.h"
#include "StaticPlatform.h"


StaticPlatform::StaticPlatform(const Rectf& rect)
	: Platform(rect)
{
}

void StaticPlatform::Draw() const
{
	// [DEBUG]
	for (size_t i = 0; i < m_RectVertices.size(); i++)
	{
		utils::SetColor(Color4f(1, 0, 0, 1));
		utils::DrawRect(m_Shape);
	}
}