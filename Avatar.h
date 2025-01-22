#pragma once
#include "utils.h"
#include "Level.h"
#include "PowerUpManager.h"

class Sprite;

class Avatar
{
public:
	enum class MarioState
	{
		idle = 0,
		walking = 1,
		jumping = 2,
		falling = 3,
		running = 4,
		runJumping = 5,
		changeDirection = 6
		//transforming = 7
	};

	Avatar();
	~Avatar();
	Avatar(const Avatar& other) = delete;
	Avatar& operator=(const Avatar& other) = delete;
	Avatar(Avatar&& other) = delete;
	Avatar& operator=(Avatar&& other) = delete;

	void Update(float elapsedSec, Level& level);
	void Draw() const;
	void StatesDrawing() const;

	// [Avatar]
	void AvatarInput(Level& level, float elapsedSec);
	void UpdatePosition(Level& level, float elapsedSec);
	void UpdateSprites(float elapsedSec, Level& level);
	void AcceleratingWalk(const Uint8* keyPress, bool isOnGround, float elapsedSec);
	void Deceleration(const Uint8* keyPress, float elapsedSec);
	void IdleDownHitboxChange(const Uint8* keyPress);

	// [Jump Mechanics]
	void Jumping(const Uint8* keyPress, bool isOnGround, float elapsedSec);
	void JumpToggle(const Uint8* keyPress, bool isOnGround);
	void CheckJumpOrFall(bool isOnGround);
 
	void PowerUpHit();

	// [Getters]
	Rectf GetMarioShape() const;
	Vector2f GetVelocity() const;

	
private:
	MarioState m_MarioState;
	Rectf m_Mario;
	Vector2f m_DrawOffset;

	const float m_BabyMarioWidth;
	const float m_BabyMarioHeight;
	float m_HorizontalSpeed;
	float m_JumpSpeed;
	float m_JumpTime;
	const float m_MaxJumpTime;

	float m_DecelerationTimer;

	Vector2f m_Velocity;
	Vector2f m_Gravity;

	float m_LastInputVelocity;
	const Vector2f m_GravityAccelaration;

	// [Animation & Sprites]
	Sprite* m_Animations;       // *map possible improvement

	bool m_IsLookingDown;
	bool m_IsJumping;
	bool m_ShouldStopJumping;

	float m_changeDirectionTimer;
	bool m_IsDirectionChanged;
};