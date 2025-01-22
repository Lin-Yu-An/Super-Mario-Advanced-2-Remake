#include "pch.h"
#include "PowerUp.h"
#include "Texture.h"
#include "Matrix2x3.h"

PowerUp::PowerUp(const Point2f& center, PowerUp::Type type)
	: m_Type{ type }
	, m_DstRect{}
{
	m_pTexture = new Texture{ "Resources/Images/Mushroom.png" };

	m_TextClip.left = 0.0f;
	m_TextClip.width = m_pTexture->GetWidth();
	m_TextClip.height = m_pTexture->GetHeight() / 2;
	if (m_Type == Type::mushroom)
	{
		m_TextClip.bottom = m_pTexture->GetHeight();
	}

	m_DstRect.left = m_Mushroom.left;
	m_DstRect.bottom = m_Mushroom.bottom;
	m_DstRect.width = m_Mushroom.width;
	m_DstRect.height = m_Mushroom.height;
}

PowerUp::~PowerUp()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PowerUp::Update(float elapsedSec)
{
}

void PowerUp::Draw() const
{	
	glPushMatrix();
	glTranslatef(m_Mushroom.left, m_Mushroom.bottom, 0);
	//glScalef(1, 1, 1);
	glTranslatef( -m_Mushroom.left, -m_Mushroom.bottom, 0);
	m_pTexture->Draw(m_DstRect, m_TextClip);
	glPopMatrix();
}

bool PowerUp::IsOverlapping(const Rectf& rect) const
{
	if (utils::IsOverlapping(rect, m_Mushroom))
	{
		return true;
	}

	return false;
}
