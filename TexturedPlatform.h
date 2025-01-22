#pragma once
#include "Platform.h"

class TexturedPlatform : public Platform
{
public:
	TexturedPlatform(const Rectf& rect, int startRow, int nrCols, float frameSec, float width, float height, const std::string& path);
	virtual ~TexturedPlatform();
	TexturedPlatform(const TexturedPlatform& other) = delete;
	TexturedPlatform& operator=(const TexturedPlatform& other) = delete;
	TexturedPlatform(TexturedPlatform&& other) = delete;
	TexturedPlatform& operator=(TexturedPlatform&& other) = delete;

	virtual void Draw() const override;
protected:
	std::string m_Path;
	Texture* m_pTexture;
	Sprite* m_pSprite;
private:
	Rectf m_SrcRect;
	Rectf m_DstRect;
};