#pragma once
#include "GameState.h"
#include <unordered_map>



struct GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();


	std::string GetGameState();

	STATE previous;
	STATE current;
	STATE next;

private:
	std::unordered_map<STATE, GameState> m_gamestates;
};