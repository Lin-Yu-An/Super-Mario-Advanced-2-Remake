#pragma once
#include "utils.h"

class Texture;

class PowerUp 
{
public:
	enum class Type 
	{  
		mushroom = 1, 
		fireFlower = 2	
	};
	explicit PowerUp(const Point2f& center, PowerUp::Type type );
	~PowerUp();
	
	void Update( float elapsedSec );
	void Draw() const;
	bool IsOverlapping(const Rectf& rect) const;

private:
	const Type m_Type;  
	const Texture* m_pTexture; 
	Rectf m_TextClip;
	Rectf m_Mushroom;
	Rectf m_DstRect;
};


