#pragma once
#include "Platform.h"

class Texture;
class Sprite;

class SpinningPlatform final: public Platform
{
public:
	SpinningPlatform(const Rectf& rect, const std::string& path, int startRow, int nrCols, float frameSec, float width, float height);
	virtual ~SpinningPlatform() = default;
	SpinningPlatform(const SpinningPlatform& other) = delete;
	SpinningPlatform& operator=(const SpinningPlatform& other) = delete;
	SpinningPlatform(SpinningPlatform&& other) = delete;
	SpinningPlatform& operator=(SpinningPlatform&& other) = delete;

	virtual void Draw() const override;
private:
	Texture* m_SpinningPlatform;
	std::vector<Sprite*> m_Animations;
	float m_SpinRadius;
	Rectf m_SrcRect;
	Rectf m_DstRect;
};