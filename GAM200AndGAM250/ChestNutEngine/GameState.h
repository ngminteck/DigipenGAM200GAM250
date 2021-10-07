#pragma once
#include <string>

enum STATE : int
{
	MAINMENU = 0,
	LEVEL1 = 1,
	QUIT   = 2,
	RESTART = 3
};

struct GameState
{
	GameState();
	GameState(std::string, STATE);
	~GameState();

	STATE m_state;
	std::string m_path;
};