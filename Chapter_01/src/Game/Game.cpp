#include "Game.h"
#include <iostream>

Game::Game()
{
	m_isRunning = true;
	m_paddlePos.x = m_thickness / 2;
	m_paddlePos.y = 720 / 2;
	m_ballPos.x = 1280 / 2;
	m_ballPos.y = 720 / 2;
}

// INITIALISE
bool Game::initialise()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	m_window = SDL_CreateWindow("Ping Pong", 200, 40, 1280, 720, 0);
	if (!m_window)
	{
		SDL_Log("Unable to create Window: %s", SDL_GetError());
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_renderer)
	{
		SDL_Log("Unable to create Renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

// MAIN LOOP
void Game::runLoop()
{
	while (m_isRunning)
	{
		processInput();
		updateGame();
		generateOutput();
	}
}

// SHUTDOWN
void Game::shutdown()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	std::cout << "FINAL SCORE: " << score << "\n\n\n";
	std::cout << "Press any key to exit...";
	std::cin.get();
}

// INPUT PROCESSING
void Game::processInput()
{
	SDL_Event event;

	// While there are still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;
		}
	}

	// Keyboard Input
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_ESCAPE])
		m_isRunning = false;

	m_paddleDir = 0;
	if (keyboardState[SDL_SCANCODE_UP])
		m_paddleDir -= 1;
	if (keyboardState[SDL_SCANCODE_DOWN])
		m_paddleDir += 1;
}

// UPDATE GAME
void Game::updateGame()
{
	float deltaTime = (SDL_GetTicks() - m_ticksCount) / 1000.0f;
	deltaTime = deltaTime > 0.5f ? 0.5f : deltaTime;
	m_ticksCount = SDL_GetTicks();

	// Paddle Movement
	if (m_paddleDir != 0)
	{
		m_paddlePos.y += m_paddleDir * m_paddleSpeed * deltaTime;

		if (m_paddlePos.y > 718 - m_paddleHeight - m_thickness)
			m_paddlePos.y = 718 - m_paddleHeight - m_thickness;
		else if (m_paddlePos.y < m_thickness)
			m_paddlePos.y = m_thickness + 2;
	}

	// Ball Movement
	timeSinceCollision += deltaTime;
	m_ballPos.x += m_ballVelocity.x * deltaTime;
	m_ballPos.y += m_ballVelocity.y * deltaTime;
	if (m_ballPos.y < (1.5f * m_thickness) && m_ballVelocity.y < 0)
	{
		m_ballVelocity.y *= -1;
		timeSinceCollision = 0.0f;
	}
	else if (m_ballPos.y > (720 - 1.5f * m_thickness) && m_ballVelocity.y > 0)
	{
		m_ballVelocity.y *= -1;
		timeSinceCollision = 0.0f;
	}
	else if (m_ballPos.x > 1280 - 1.5f * m_thickness && m_ballVelocity.x > 0)
	{
		m_ballVelocity.x *= -1;
		timeSinceCollision = 0.0f;
	}

	if (m_ballPos.y + m_thickness >= m_paddlePos.y && m_ballPos.y <= m_paddlePos.y + m_paddleHeight && m_ballPos.x <= (m_paddlePos.x + m_thickness) && m_ballPos.x >= m_paddlePos.x && m_ballVelocity.x < 0)
	{
		m_ballVelocity.x *= -1;
		timeSinceCollision = 0.0f;
		score += 1;
	}

	if (m_ballPos.x < 0)
		m_isRunning = false;

	// making sure the window refreshes at fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + (1.0f / m_fps) * 1000));
}

#define WALL_COLOR 80, 80, 80, 255

// GENERATE OUTPUT
void Game::generateOutput()
{
	// Clear color
	SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, 255);
	SDL_RenderClear(m_renderer);

	// Draw WALLS
	SDL_SetRenderDrawColor(m_renderer, WALL_COLOR);
	SDL_Rect topWall{ 0, 0, 1280, m_thickness };
	SDL_RenderFillRect(m_renderer, &topWall);
	SDL_SetRenderDrawColor(m_renderer, WALL_COLOR);
	SDL_Rect rightWall{ 1280 - m_thickness, 0, m_thickness, 720 };
	SDL_RenderFillRect(m_renderer, &rightWall);
	SDL_SetRenderDrawColor(m_renderer, WALL_COLOR);
	SDL_Rect bottomWall{ 0, 720 - m_thickness, 1280, m_thickness };
	SDL_RenderFillRect(m_renderer, &bottomWall);

	// Draw paddle and ball
	if (timeSinceCollision < 0.15)
		SDL_SetRenderDrawColor(m_renderer, 255, 40, 40, 255);
	else
		SDL_SetRenderDrawColor(m_renderer, 255, 140, 0, 255);

	SDL_Rect paddle{m_paddlePos.x, m_paddlePos.y, m_thickness, m_paddleHeight};
	SDL_RenderFillRect(m_renderer, &paddle);

	SDL_Rect ball{ static_cast<int>(m_ballPos.x - m_thickness /2), static_cast<int>(m_ballPos.y - m_thickness /2), m_thickness, m_thickness };
	SDL_RenderFillRect(m_renderer, &ball);

	// Swap buffers
	SDL_RenderPresent(m_renderer);
}
