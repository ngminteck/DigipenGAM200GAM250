#pragma once
#include "GameEngine.h"
#include "GameStateManager.h"
#include <iostream>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

class Overthrone
{
public:
	Overthrone(GameEngine *, GameStateManager *);
	~Overthrone();

	//Game main functions
	void RunGame(MSG &, bool &);
	void ExitGame();

	//Game sub functions
	int Init();
	void InitializeCamera();
	void Load(std::string);
	void Update(MSG &, bool &);
	void Unload();

private:
	GameEngine * m_engine;
	GameStateManager * m_gamestate;
	//MainMenu  m_menu;
};