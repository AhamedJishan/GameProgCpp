#pragma once
#include <SDL.h>

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	int score = 0;
	Game();
	bool initialise();
	void runLoop();
	void shutdown();

private:
	void processInput();
	void updateGame();
	void generateOutput();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_isRunning;

	Vector2 m_paddlePos;
	int m_paddleHeight = 120;
	int m_paddleDir = 0;
	float m_paddleSpeed = 300.0f;

	Vector2 m_ballPos;
	Vector2 m_ballVelocity {-220.0f, 200.0f};

	float timeSinceCollision = 10.0f;

	const int m_thickness = 20;
	Uint32 m_ticksCount;
	float m_fps = 60.0f;
};