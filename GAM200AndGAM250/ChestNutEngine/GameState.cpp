#include "GameState.h"

GameState::GameState()
{
}

GameState::GameState(std::string path, STATE state)
	:m_path{path}, m_state{state}
{
}

GameState::~GameState() {}


