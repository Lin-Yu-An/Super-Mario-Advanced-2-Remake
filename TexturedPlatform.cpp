#include "pch.h"
#include "TexturedPlatform.h"
#include "Texture.h"

TexturedPlatform::TexturedPlatform(const Rectf& rect, int startRow, int nrCols, float frameSec, float width, float height, const std::string& path)
	: Platform(rect, startRow, nrCols, frameSec, width, height)
	, m_SrcRect{ 0, 0, 0, 0}
	, m_DstRect{ 0, 0, 0, 0}
{
	m_pTexture = new Texture{ path };
	m_pSprite = new Sprite{ m_pTexture, startRow, nrCols, frameSec, width, height };

	m_SrcRect.left = 0;
	m_SrcRect.bottom = 0;
	m_SrcRect.width = width;
	m_SrcRect.height = height;

	m_DstRect.left = 50.f;
	m_DstRect.bottom = 100.f;
	m_DstRect.width = m_SrcRect.width;
	m_DstRect.height = m_SrcRect.height;
}

TexturedPlatform::~TexturedPlatform()
{
	delete m_pTexture;
	m_pTexture = nullptr;

	delete m_pSprite;
	m_pSprite = nullptr;
}


void TexturedPlatform::Draw() const
{
	m_pTexture->Draw(m_DstRect, m_SrcRect);
}

 
