#pragma once
#include <string>

class Texture;

class Sprite final
{
public:
	explicit Sprite( const std::string& path, int cols = 1, int rows = 1, float frameSec = 0 );
	explicit Sprite( Texture* texture, const std::string& path, int cols = 1, int rows = 1, float frameSec = 0 );
	~Sprite( );
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);
	Sprite(Sprite&& rhs) noexcept;
	Sprite& operator=(Sprite&& rhs) noexcept;
	
	void Update( float elapsedSec );
	void Draw( const Point2f& pos, float scale = 1.0f ) const;
	void Draw(const Rectf& destRect) const;

	void SkipFrames(int frameAmount);
	void SetCurrentFrame(int frame);
	void SetFrameSec(float frameSec);
	
	float GetFrameWidth( ) const;
	float GetFrameHeight( ) const;
	int GetCurrentFrame() const;
	int GetTotalFrames() const;
	Rectf GetBounds(Point2f pos, float scale) const;

private:
	Texture *m_pTexture;
	std::string m_Path;
	int m_Cols;
	int m_Rows;
	float m_FrameSec;
	float m_AccuSec;
	int m_CurrentFrame;
	bool m_Static;
	
};


