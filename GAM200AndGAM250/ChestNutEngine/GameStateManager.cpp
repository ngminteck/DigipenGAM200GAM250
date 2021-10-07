#include "GameStateManager.h"

GameStateManager::GameStateManager()
	:previous{ LEVEL1 }, current { LEVEL1 }, next{ LEVEL1 }
{
	GameState Level1{ "Resource/Json/Level/MainMenu.json", QUIT};
	m_gamestates.insert(std::make_pair(LEVEL1, Level1));
	m_gamestates[QUIT];
}

GameStateManager::~GameStateManager()
{

}

std::string GameStateManager::GetGameState()
{
	if (m_gamestates.find(current) != m_gamestates.end())
	{
		return m_gamestates[current].m_path;
	}

	else
		return std::string{};
}
