#pragma once
#include "System.h"
#include "ButtonComponent.h"
#include "EventHandler.h"
#include "SceneActorFactory.h"
#include "LoadLevel.h"
#include "Input.h"
#include <vector>
#include <unordered_map>
#include "LoadGameState.h"
#include "SpriteSheetFactory.h"
#include "QuitGame.h"
#include "PlayAudio.h"
#include "XBoxInputSystem.h"
#include "RestartLevel.h"

class ButtonSystem : System
{
public:
	ButtonSystem(XBoxInputSystem *, InputSystem *, SceneActorFactory *, EventHandler *, SpriteSheetFactory *, bool &, float &);
	~ButtonSystem();
	void Initialize();
	void Update();
	void ClearSelection();

	void ActivateButton(unsigned, unsigned);
	void ActivateMenu(int);
	void HoverButton(unsigned, unsigned);


	void ReInitialize(LoadLevel *);
	void DeactivateMenu(int);
	void TogglePauseMenu(bool);
	void ActivateDeathMenu();
	void UpdateVolumeIcon();
	void CollapseMenu();
	void ReloadMenu(unsigned section, unsigned index);

	int selected;
	int m_currentMenu;
	int m_prevMenu;
	int m_currentSection;

	std::unordered_map<unsigned, SceneActor *> m_volume;
private:
	InputSystem * m_input;
	SceneActorFactory * m_fac;
	EventHandler * m_ev;
	SpriteSheetFactory * m_spriteFac;
	bool & m_pause;
	float & m_multiplier;
	XBoxInputSystem * m_xbox;

	std::unordered_map<unsigned, std::unordered_map<unsigned, ButtonComponent *>> m_buttons_;
	std::unordered_map<unsigned, std::unordered_map<unsigned, SceneActor *>> m_hovered_;
	std::unordered_map<unsigned, std::unordered_map<unsigned, SceneActor *>> m_notHovered_;
};