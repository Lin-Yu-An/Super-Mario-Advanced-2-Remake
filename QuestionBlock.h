#pragma once
#include "Platform.h"

class Texture;
class Sprite;
class Level;

class QuestionBlock final : public Platform
{
	enum class BlockState
	{
		beforeHit,
		isHit
	};
	
public:
	explicit QuestionBlock(Point2f pos);
	QuestionBlock(const QuestionBlock& other) = delete;
	QuestionBlock& operator=(const QuestionBlock& other) = delete;
	QuestionBlock(QuestionBlock&& other) = delete;
	QuestionBlock& operator=(QuestionBlock&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, Level& level) override;
	void SpawnMushroom(Level& level);
	bool IsHit();
private:
	BlockState m_BlockState;
	bool m_IsHit;
	bool m_IsSpawned;
};