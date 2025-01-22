#include "pch.h"
#include "Sprite.h"
#include <iostream>
#include "Texture.h"

Sprite::Sprite( const std::string& path, int cols, int rows, float frameSec )
	: m_pTexture{ new Texture( path )}
	, m_Path{ path }
	, m_Cols( cols )
	, m_Rows( rows )
	, m_FrameSec( frameSec )
	, m_AccuSec{}
	, m_CurrentFrame{}
	, m_Static{false}
{
}

Sprite::Sprite(Texture* texture, const std::string& path, int cols, int rows, float frameSec)
	: m_pTexture{ texture }
	, m_Path{ path }
	, m_Cols(cols)
	, m_Rows(rows)
	, m_FrameSec(frameSec)
	, m_AccuSec{}
	, m_CurrentFrame{}
	, m_Static{true}
{
}

Sprite::~Sprite( )
{
	if (m_Static) return;
	delete m_pTexture;
}

Sprite::Sprite(const Sprite& rhs)
	:m_pTexture{ new Texture(rhs.m_Path) }
	,m_Path{rhs.m_Path}
	,m_Cols{rhs.m_Cols}
	,m_Rows(rhs.m_Rows)
	,m_FrameSec(rhs.m_FrameSec)
	,m_AccuSec{rhs.m_FrameSec}
	,m_CurrentFrame{rhs.m_CurrentFrame}
	,m_Static{ rhs.m_Static }
{
}

Sprite& Sprite::operator=(const Sprite& rhs)
{
	if (&rhs != this)
	{
		delete m_pTexture;
		m_pTexture = new Texture( rhs.m_Path);
		//m_pBombTexture = rhs.m_pBombTexture;
		m_Path = rhs.m_Path;
		m_Cols = rhs.m_Cols;
		m_Rows = rhs.m_Rows;
		m_FrameSec = rhs.m_FrameSec;
		m_AccuSec = rhs.m_AccuSec;
		m_CurrentFrame = rhs.m_CurrentFrame;
		m_Static = false;
	}
	return *this;
}

Sprite::Sprite(Sprite&& rhs) noexcept
	:m_pTexture{ rhs.m_pTexture }
	,m_Path{ rhs.m_Path }
	,m_Cols{ rhs.m_Cols }
	,m_Rows(rhs.m_Rows)
	,m_FrameSec(rhs.m_FrameSec)
	,m_AccuSec{ rhs.m_FrameSec }
	,m_CurrentFrame{ rhs.m_CurrentFrame }
	,m_Static{ rhs.m_Static }
	//,m_pBombTexture{ new Texture(rhs.m_Path) }
{
	rhs.m_pTexture = nullptr;
	//rhs.m_Path = nullptr;
	rhs.m_Cols = 0;
	rhs.m_Rows = 0;
	rhs.m_FrameSec = 0;
	rhs.m_AccuSec = 0;
	rhs.m_CurrentFrame = 0;
}

Sprite& Sprite::operator=(Sprite&& rhs) noexcept
{
	if (&rhs != this)
	{
		delete m_pTexture;
		//m_pBombTexture = new Texture( rhs.m_Path );
		m_pTexture = rhs.m_pTexture;
		m_Path = rhs.m_Path;
		m_Cols = rhs.m_Cols;
		m_Rows = rhs.m_Rows;
		m_FrameSec = rhs.m_FrameSec;
		m_AccuSec = rhs.m_AccuSec;
		m_CurrentFrame = rhs.m_CurrentFrame;
		m_Static = rhs.m_Static;
		
		rhs.m_pTexture = nullptr;
		//rhs.m_Path= nullptr;
		rhs.m_Cols= 0;
		rhs.m_Rows= 0;
		rhs.m_FrameSec= 0;
		rhs.m_AccuSec= 0;
		rhs.m_CurrentFrame= 0;
	}
	return *this;
}

void Sprite::Update( float elapsedSec )
{
	m_AccuSec += elapsedSec;

	if ( m_AccuSec > m_FrameSec )
	{
		// Go to next frame
		++m_CurrentFrame;
		if ( m_CurrentFrame >= m_Cols * m_Rows )
		{
			m_CurrentFrame = 0;
		}

		// Only keep the remaining time
		m_AccuSec -= m_FrameSec;
	}
}

void Sprite::Draw( const Point2f& pos, float scale ) const
{
	// frame dimensions
	const float frameWidth{  m_pTexture->GetWidth() / m_Cols };
	const float frameHeight{  m_pTexture->GetHeight() / m_Rows };
	int row = m_CurrentFrame / m_Cols;
	int col = m_CurrentFrame % m_Cols;

	Rectf srcRect;
	srcRect.height = frameHeight;
	srcRect.width = frameWidth;
	srcRect.left = m_CurrentFrame % m_Cols * srcRect.width;
	srcRect.bottom = m_CurrentFrame / m_Cols * srcRect.height + srcRect.height;
	const Rectf destRect{ pos.x, pos.y,srcRect.width * scale,srcRect.height * scale };
	m_pTexture->Draw( destRect, srcRect );
}

void Sprite::Draw(const Rectf& destRect) const
{
	//part of texture that corresponds with the current frame number
	Rectf srcRect{};
	srcRect.width = GetFrameWidth();
	srcRect.height = GetFrameHeight();
	srcRect.left = (m_CurrentFrame % m_Cols) * srcRect.width;
	srcRect.bottom = (m_CurrentFrame / m_Cols + 1) * srcRect.height;

	m_pTexture->Draw(destRect, srcRect);
}

void Sprite::SkipFrames(int frameAmount)
{
	//determine next frame number
	const int mod = (m_CurrentFrame + frameAmount) % GetTotalFrames();

	if (mod < 0) m_CurrentFrame = GetTotalFrames() + mod;
	else m_CurrentFrame = mod;
}

void Sprite::SetCurrentFrame(int frame)
{
	//determine next frame number
	const int mod = frame % GetTotalFrames();

	if (mod < 0) m_CurrentFrame = GetTotalFrames() + mod;
	else m_CurrentFrame = mod;
}

void Sprite::SetFrameSec(float frameSec)
{
	m_FrameSec = frameSec;
}

float Sprite::GetFrameWidth( ) const
{
	return m_pTexture->GetWidth( ) / m_Cols;
}

float Sprite::GetFrameHeight( ) const
{
	return m_pTexture->GetHeight( ) / m_Rows;
}

int Sprite::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

int Sprite::GetTotalFrames() const
{
	return m_Cols * m_Rows;
}

Rectf Sprite::GetBounds(Point2f pos, float scale) const
{
	return Rectf(pos.x, pos.y, (this->m_pTexture->GetWidth() / this->m_Cols) * scale, this->m_pTexture->GetHeight() / (this->GetTotalFrames() / this->m_Cols) * scale);
}
