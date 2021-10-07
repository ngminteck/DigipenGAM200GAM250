#pragma once
#include "System.h"
#include <vector>
#include "SceneActor.h"
#include "EventHandler.h"
#include "CollisionComponent.h"
#include "SpriteSheetFactory.h"
#include "LoadLevel.h"
#include "DialogueComponent.h"
#include "Texture2D.h"
#include "Input.h"
#include "XBoxInputSystem.h"

class DialogueSystem : System
{
public:
	DialogueSystem(XBoxInputSystem *,InputSystem *, EventHandler *, std::map<unsigned, SceneActor*> *, SpriteSheetFactory *, bool &);
	~DialogueSystem();

	void Initialize();
	void Reinitialize(LoadLevel *);
	void Update();
	void ActivateNextDialogue();

	bool m_dialogueMode = false;

private:

	XBoxInputSystem * m_xbox;
	InputSystem * m_input;
	EventHandler * m_ev;
	SpriteSheetFactory * m_spritefac;

	SceneActor * m_nextbutton;
	SpriteComponent * m_nextbutton_sprite;

	SpriteComponent * m_dialogue_box;
	SceneActor * m_dialogue_box_obj;

	std::map<unsigned, SceneActor *> m_collider_obj;

	std::map<unsigned, std::vector<Texture2D * >> m_dialogues;
	std::map<unsigned, SceneActor*> * m_objs;
	unsigned m_currDialogue;
	unsigned m_currID = 0;

	bool & m_pause;
	
	std::map<unsigned, CollisionComponent *> m_collider;
};