#pragma once
#include "Level.h"
#include "Avatar.h"
#include "Camera.h"
#include "PowerUpManager.h"

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// [Event handling]
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// [DATA MEMBERS]
	const Window m_Window;

	Level m_Level;
	Avatar m_Mario;
	Camera m_Camera;
	PowerUpManager m_PowerUpManager;

	// [FUNCTIONS]
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void AddPowerUps();
	void DoCollisionTests();
};