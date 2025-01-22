#include "pch.h"
#include "Avatar.h"

#include <iostream>

#include "Sprite.h"

Avatar::Avatar()
	: m_MarioState{ MarioState::idle }							//Indicates the current action state for Mario
	, m_Mario{20.f, 100.f, 12.f, 20.f}	    //The rect shape that contains the position and dimensions of the avater
	, m_DrawOffset{ 0.0f, -3.0f }							//Offset from hitbox's bottomleft pos to sprite's bottomleft pos
    , m_BabyMarioWidth{ 16.f }
    , m_BabyMarioHeight{ 22.f }
	, m_HorizontalSpeed{ 100.f }								//horizontal velocity when moving
	, m_JumpSpeed{ 300.f }
	, m_JumpTime{ 0 }
	, m_MaxJumpTime{ 0.25f }
	, m_DecelerationTimer{ 0 }
	, m_Velocity{ 0.0f, 0.0f }
	, m_Gravity{ 0, -800.f }
	, m_LastInputVelocity{ 0 }
    , m_IsLookingDown{ false }
	, m_IsJumping{ false }
	, m_ShouldStopJumping{ false }
	, m_IsDirectionChanged{ false }
    , m_changeDirectionTimer{ 0 }
{
	//m_pSpritesTexture = new Texture{ "Resources/Images/BabyMarioSprites.png" };
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 0, 1, 0.2f, m_BabyMarioWidth, m_BabyMarioHeight)); // idle
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 3, 2, 0.1f, m_BabyMarioWidth, m_BabyMarioHeight)); // walking
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 6, 1, 0.2f, m_BabyMarioWidth, m_BabyMarioHeight)); // jumping
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 7, 1, 0.2f, m_BabyMarioWidth, m_BabyMarioHeight)); // falling
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 4, 2, 0.1f, m_BabyMarioWidth, m_BabyMarioHeight)); // running
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 9, 1, 0.2f, m_BabyMarioWidth, m_BabyMarioHeight)); // runJumping
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 5, 1, 0.2f, m_BabyMarioWidth, m_BabyMarioHeight)); // changeDirection
// 
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 1, 1, 0.2f, m_BabyMarioWidth, m_BabyMarioHeight)); // idleUp
// 	m_Animations.push_back(new Sprite(m_pSpritesTexture, 2, 1, 0.2f, m_BabyMarioWidth, m_BabyMarioHeight)); // idleDown

	m_Animations = new Sprite("Resources/Images/BabyMarioSprite2.png", 4, 10, 0.2f);
}

Avatar::~Avatar()
{
	delete m_Animations;
	m_Animations = nullptr;
}


void Avatar::Update(float elapsedSec, Level& level)
{
	// [Update position through keyboard inputs]
	AvatarInput(level, elapsedSec);

	// [Update mario's position] 
	UpdatePosition(level, elapsedSec);
	
	// [Update mario's sprite]
	m_Animations->Update(elapsedSec);
	UpdateSprites(elapsedSec, level);

	// [Handle level collision]
	level.HandleCollision(m_Mario, m_Velocity);
}


void Avatar::Draw() const
{
	glPushMatrix();
		StatesDrawing();
	glPopMatrix();
}


void Avatar::AvatarInput(Level& level, float elapsedSec)
{
	// [Check keyboard state]
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	
	// [Check Mario hitbox]
	bool isOnGround{ level.IsOnGround(m_Mario, m_Velocity) };
	bool isHittingWall{ level.IsHittingWall(m_Mario, m_Velocity) };

	// [DEBUG]:
	//std::cout << " IsOnGround:" << isOnGround << std::endl;
	//std::cout << "CurrentVelocity:" << m_Velocity << std::endl;
	std::cout << "CurrentState:" << static_cast<int>(m_MarioState) << std::endl;
	//std::cout <<  m_Velocity.x << ", " << m_Velocity.y << '\n';
	//std::cout << m_Gravity << '\n';

	// [Moving Left]
	if (pStates[SDL_SCANCODE_LEFT])
	{
		if (m_changeDirectionTimer == 0 && m_LastInputVelocity > 0)
		{
			m_IsDirectionChanged = true;
		}
		
		m_MarioState = MarioState::walking;
		m_Velocity.x = -m_HorizontalSpeed;
		m_LastInputVelocity = m_Velocity.x;		//Memorize last input
	}
	// [Moving Right]
	else if (pStates[SDL_SCANCODE_RIGHT])
	{
		if (m_changeDirectionTimer == 0 && m_LastInputVelocity < 0)
		{
			m_IsDirectionChanged = true;
		}
		
		m_MarioState = MarioState::walking;
		m_Velocity.x = m_HorizontalSpeed;
		m_LastInputVelocity = m_Velocity.x;      //Memorize last input

	}
	// [Stop Motion && Deceleration] 
	else if((!pStates[SDL_SCANCODE_LEFT] || !pStates[SDL_SCANCODE_RIGHT]))
	{
		Deceleration(pStates, elapsedSec);
		
		if (isOnGround || isHittingWall)
		{
			m_MarioState = MarioState::idle;
		}
	}

	
	// [ChangeDirection]
	if (m_IsDirectionChanged && (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_RIGHT]))
	{
		m_changeDirectionTimer += elapsedSec;

		if (m_changeDirectionTimer >= 0.1f)
		{
			m_changeDirectionTimer = 0;
			m_IsDirectionChanged = false;
		}
		else
		{
			m_MarioState = MarioState::changeDirection;

			if (pStates[SDL_SCANCODE_LEFT])
			{
				m_Velocity.x = -10.f;
			}
			else
			{
				m_Velocity.x = 10.f;
			}
		}
	}

	AcceleratingWalk(pStates, isOnGround, elapsedSec);

	IdleDownHitboxChange(pStates);

	Jumping(pStates, isOnGround, elapsedSec);

	CheckJumpOrFall(isOnGround);
}

void Avatar::UpdatePosition(Level& level, float elapsedSec)
{
	if (!level.IsOnGround(m_Mario, m_Velocity))
	{
		m_Velocity += m_Gravity * elapsedSec;
	}
	
	m_Mario.left += m_Velocity.x * elapsedSec;
	m_Mario.bottom += m_Velocity.y * elapsedSec;
}

void Avatar::UpdateSprites(float elapsedSec, Level& level)
{
	m_Animations->Update(elapsedSec);
}

void Avatar::AcceleratingWalk(const Uint8* keyPress, bool isOnGround, float elapsedSec)
{
	//[Running]
	if (keyPress[SDL_SCANCODE_X] && m_MarioState == MarioState::walking)
	{
		if (m_HorizontalSpeed <= 200.f)
		{
			m_HorizontalSpeed += 1.f;  //accelerate
		}
	}
	else
	{
		//reset speed when not pressing anymore
		m_HorizontalSpeed = 100.f;    
	}

	//Check speed to switch walking to running
	if (m_HorizontalSpeed >= 200.f )
	{
		m_MarioState = MarioState::running;
		
		if (m_IsJumping)
		{
			m_Gravity.y = -600.f;
		}
		else
		{
			m_Gravity.y = -800.f;
		}
	}
}

void Avatar::Deceleration(const Uint8* keyPress, float elapsedSec)
{
	m_DecelerationTimer += 12.f * elapsedSec;

	// [Moving Left]
	if (m_Velocity.x < 0.f)
	{
		m_Velocity.x += m_DecelerationTimer;

		if (m_Velocity.x >= -0.1f)
		{
			m_MarioState = MarioState::idle;
			m_Velocity.x = 0;
			m_DecelerationTimer = 0;
		}
	}
	// [Moving Right]
	else if (m_Velocity.x > 0.f)
	{
		m_Velocity.x -= m_DecelerationTimer;

		if (m_Velocity.x <= 0.1f)
		{
			m_MarioState = MarioState::idle;
			m_Velocity.x = 0;
			m_DecelerationTimer = 0;
		}
	}
}

void Avatar::IdleDownHitboxChange(const Uint8* keyPress)
{
	if (keyPress[SDL_SCANCODE_DOWN])
	{
		//Height of the hitbox cut in half when duck down
		m_Mario.height = 10.f;
	}
	else
	{
		m_Mario.height = 20.f;
	}
}

void Avatar::Jumping(const Uint8* keyPress, bool isOnGround, float elapsedSec)
{
	// [Tap && Hold Jump]
	JumpToggle(keyPress, isOnGround);
	
	if(m_IsJumping)
	{
		//start the timer
		m_JumpTime += elapsedSec;
		
		//check if shouldStopJumping true && timer is above minimum jumptime
		if (m_ShouldStopJumping)
		{
			m_ShouldStopJumping = false;
			m_IsJumping = false;
			m_Velocity.y = 0;
			m_JumpTime = 0;
		}
		else
		{
			if (keyPress[SDL_SCANCODE_Z])
			{
				//apply jumping velocity
				m_MarioState = MarioState::jumping;
				m_Velocity.y = m_JumpSpeed;
			}
			else if (!keyPress[SDL_SCANCODE_Z] && (m_JumpTime < m_MaxJumpTime && m_JumpTime > m_MaxJumpTime / 2.f))
			{
				m_ShouldStopJumping = true;
			}
		}
	}

	// [WalkJumping]
	if ((keyPress[SDL_SCANCODE_LEFT] || keyPress[SDL_SCANCODE_RIGHT]) && m_MarioState == MarioState::walking)
	{
		if (keyPress[SDL_SCANCODE_Z])
		{
			m_MarioState = MarioState::jumping;
		}
	}

	// [RunJumping]
	if ((keyPress[SDL_SCANCODE_LEFT] || keyPress[SDL_SCANCODE_RIGHT]) && m_MarioState == MarioState::running)
	{
		if (keyPress[SDL_SCANCODE_Z])
		{
			m_MarioState = MarioState::runJumping;
			m_HorizontalSpeed = 201.f;				//Remains at max acceleration speed.
		}
	}
}

void Avatar::JumpToggle(const Uint8* keyPress, bool isOnGround)
{
	// [Key Down]
	if (!m_IsJumping && keyPress[SDL_SCANCODE_Z] && isOnGround)
	{
		//start jumping
		m_IsJumping = true;
	}
	
	// [Key Up]
	if (m_IsJumping)
	{
		//check if we have been jumping long enough
		if (m_JumpTime >= m_MaxJumpTime)
		{
			m_ShouldStopJumping = true;
		}
	}
}


void Avatar::CheckJumpOrFall(bool isOnGround)
{
	if (m_Velocity.y > 0)
	{
		m_MarioState = MarioState::jumping;
	}
	else if (m_Velocity.y < 0 && !isOnGround)
	{
		m_MarioState = MarioState::falling;
	}
}


void Avatar::StatesDrawing() const
{
	// [DEBUG]: DrawCollision
	utils::SetColor(Color4f{ 1.0f,0.0f,0.0f,0.5f });
	utils::DrawRect(m_Mario, 2.0f);

	const float spriteOffSet{ 0.5f };
	float direction{ m_LastInputVelocity >= 0 ? 1.0f : -1.0f };
	
	Vector2f drawPos{ m_Mario.left + m_Mario.width / 2.0f - (m_Animations->GetFrameWidth() / 2) , m_Mario.bottom };
	drawPos += m_DrawOffset;

	glPushMatrix();
 	glTranslatef(drawPos.x, drawPos.y, 0); //Normal positioning
 	glTranslatef((m_Animations->GetFrameWidth() / 2) - spriteOffSet, (m_Animations->GetFrameHeight() / 2), 0);   // Pivot back
 	glScalef(direction, 1, 1);
 	glTranslatef(-(m_Animations->GetFrameWidth() / 2) - spriteOffSet, -(m_Animations->GetFrameHeight() / 2), 0); // Pivot to
	glTranslatef(-drawPos.x, -drawPos.y, 0);

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	// [IdleUp]
	if (pStates[SDL_SCANCODE_UP] && m_MarioState == MarioState::idle)
	{
		//m_Animations[static_cast<int>(MarioState::idle) + 8]->Draw(m_Mario.GetBottomLeft()); //TODO: Maybe tweaking
		m_Animations->SetCurrentFrame(4);
	}
	// [IdleDown]
	else if (pStates[SDL_SCANCODE_DOWN] && m_MarioState == MarioState::idle)
	{
		//m_Animations[static_cast<int>(MarioState::idle) + 9]->Draw(m_Mario.GetBottomLeft()); //TODO: same here
		m_Animations->SetCurrentFrame(8);
	}
	// [Everything else] *setting current frame according to the sprite sheets
	else
	{
		switch (m_MarioState)
		{
			case MarioState::idle:
				m_Animations->SetCurrentFrame(0);
			break;
			case MarioState::walking:
				if (m_Animations->GetCurrentFrame() < 12 || m_Animations->GetCurrentFrame() > 13)
				{
					m_Animations->SetCurrentFrame(12);
				}
			break;
			case MarioState::jumping:
				m_Animations->SetCurrentFrame(24);
			break;
			case MarioState::falling:
				m_Animations->SetCurrentFrame(28);
			break;
			case MarioState::running:
				if (m_Animations->GetCurrentFrame() < 16 || m_Animations->GetCurrentFrame() > 17)
				{
					std::cout << "is running " << '\n';
					m_Animations->SetCurrentFrame(16);
				}
			break;
			case MarioState::runJumping:
				m_Animations->SetCurrentFrame(36);
			break;
			case MarioState::changeDirection:
				m_Animations->SetCurrentFrame(20);
			break;
		default: ;
		}
	}
	
	//else if (pStates[SDL_SCANCODE_DOWN] && m_MarioState == MarioState::idle)
	//{
	//	//m_Animations[static_cast<int>(m_MarioState)]->Draw(m_Mario.GetBottomLeft());
	//}

	m_Animations->Draw(m_Mario.GetBottomLeft());
	glPopMatrix();
}

void Avatar::PowerUpHit()
{
	//tells the avater that it hit a power-up
	std::cout << "Power-up Get!" << '\n';
	//m_MarioState = MarioState::transforming;
}

Rectf Avatar::GetMarioShape() const
{
	return m_Mario;
}

Vector2f Avatar::GetVelocity() const
{
	return m_Velocity;
}


