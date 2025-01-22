#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_Level{}
	, m_Mario{}
	, m_Camera{ window.width, window.height }
    , m_PowerUpManager{}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	AddPowerUps();
	const Rectf levelBoundaries{ 0, 0, 5400, 632}; 
	m_Camera.SetLevelBoundaries(levelBoundaries);
}

void Game::Cleanup( )
{
}

void Game::Update( const float elapsedSec )
{
	m_PowerUpManager.Update(elapsedSec);
	m_Mario.Update(elapsedSec, m_Level);
	m_Level.Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	glPushMatrix();
		glScalef(2, 2, 0);
		m_Camera.Transform(m_Mario.GetMarioShape());
		m_Level.DrawBackGround();
		m_Level.DrawForeGround();
		// m_PowerUpManager.Draw();
		m_Mario.Draw();
	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	/*switch (e.keysym.sym)
		{
		case SDLK_z:
			std::cout << "jump key pressed\n";
			m_Mario.Jumping(m_Level.IsOnGround(m_Mario, ))
			break;
		}*/
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::AddPowerUps()
{
	m_PowerUpManager.AddItem(Point2f{ 100.0f, 200 - 183.0f }, PowerUp::Type::mushroom);
}

void Game::DoCollisionTests()
{
	if (m_PowerUpManager.HitItem(m_Mario.GetMarioShape()))
	{
		m_Mario.PowerUpHit();
	}
}