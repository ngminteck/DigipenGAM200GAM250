#include "DialogueSystem.h"

DialogueSystem::DialogueSystem(XBoxInputSystem * xbox, InputSystem * input, EventHandler * ev, std::map<unsigned, SceneActor*> * objs, SpriteSheetFactory * spritefac, bool & pause)
	:m_ev{ev}, m_objs{objs}, m_spritefac{spritefac}, m_xbox{xbox}, m_input{input}, m_nextbutton{nullptr},  m_dialogue_box{nullptr}, m_pause{pause},
	m_dialogue_box_obj{nullptr}, m_nextbutton_sprite{nullptr}, m_currID{0}
{
	m_ev->SubscribeEvent(this, &DialogueSystem::Reinitialize);
}

DialogueSystem::~DialogueSystem()
{
}

void DialogueSystem::Initialize()
{
	auto sprites = m_spritefac->GetContainer();

	for (auto & obj : *m_objs)
	{
		if (obj.second->GetSceneActorName() == "Dialogue Next")
		{
			m_nextbutton = obj.second;

			Component * comp = obj.second->GetComponent(ComponentType::SpriteComponent);
			if (comp)
			{
				SpriteComponent * sprite = dynamic_cast<SpriteComponent *>(comp);
				m_nextbutton_sprite = sprite;
				std::cout << "Dialogue system: Next button initialized" << std::endl;
			}
		}

		//Initialize the dialogue box object
		if (obj.second->GetSceneActorName() == "Dialogue Box")
		{
			Component * comp = obj.second->GetComponent(ComponentType::SpriteComponent);
			if (comp)
			{
				SpriteComponent * sprite = dynamic_cast<SpriteComponent *>(comp);
				m_dialogue_box = sprite;
				m_dialogue_box->SetTexture(nullptr);
				m_dialogue_box_obj = obj.second;
				obj.second->SetOnlyActive(false);
				std::cout << "active is now false" << std::endl;
				std::cout << "Dialogue system: Dialogue box initialized" << std::endl;
			}
		}

		//Intialize the textures of the dialogues and the collider box
		else if (obj.second->GetComponent(ComponentType::DialogueComponent) != nullptr)
		{
			DialogueComponent * dialogue = dynamic_cast<DialogueComponent *>(obj.second->GetComponent(ComponentType::DialogueComponent));
			unsigned id = dialogue->m_id;
			m_collider_obj[id] = obj.second;
			
			std::vector<std::string> text = dialogue->m_dialogues;
			for (int i = 0; i < text.size(); ++i)
			{
				unsigned texname = GetSpriteKeyBySpriteName(sprites, text[i]);
				Texture2D * texture = (*sprites)[texname];
				m_dialogues[id].push_back(texture);
			}
			std::cout << "Dialogue system: Dialogues initialized" << std::endl;

			Component * colcomp = obj.second->GetComponent(ComponentType::CollisionComponent);
			if (colcomp)
			{
				CollisionComponent * col = dynamic_cast<CollisionComponent *>(colcomp);
				m_collider[id] = col;

				std::cout << "Dialogue system: Collider box initialized" << std::endl;
			}
		}
	}
}

void DialogueSystem::Reinitialize(LoadLevel *)
{
	m_dialogues.clear();
	m_dialogueMode = false;
	m_currID = 0;
	m_currDialogue = 0;
	Initialize();
}

void DialogueSystem::Update()
{
	//if it is collided by an object
	if (!m_dialogueMode)
	{
		
		for (int i = 0; i < m_collider_obj.size(); ++i)
		{
			//If the object is active
			if (m_collider_obj[i]->GetActive())
			{
				if (m_collider[i]->GetIsColliding())
				{
					auto objs = m_collider[i]->GetVecOfPtrToCollidedObject();
					for (auto & elem : objs)
					{
						if (elem->GetSceneActorName() == "Surtur")
						{
							//Activate the dialogue
							if (m_dialogues[m_currID].size() > 0 && m_dialogue_box)
							{
								Texture2D * tex = m_dialogues[m_currID][m_currDialogue];
								m_dialogue_box->SetTexture(tex);
								m_dialogue_box_obj->SetOnlyActive(true);
								m_dialogueMode = true;
								std::cout << "Texture set";

								//Activate the button
								if (m_nextbutton)
									m_nextbutton->SetOnlyActive(true);

								//pause the game
								m_pause = !m_pause;
							}

							break;
						}
					}
				}
			}
		}
	}

	if (m_dialogueMode)
	{
		if (m_xbox->isKeyTriggered(BUTTON_A) || m_input->InputCheckTriggered(CVK_ENTER) || m_input->InputCheckTriggered(CVK_SPACE) || m_input->InputCheckTriggered(CVK_K) || m_input->InputCheckTriggered(CVK_L))
		{
			ActivateNextDialogue();
		}
	}
}

void DialogueSystem::ActivateNextDialogue()
{
	//If it is the last dialogue
	if (m_currDialogue == m_dialogues[m_currID].size() - 1)
	{
		//unpause the game
		m_pause = !m_pause;

		//Off the dialogue box
		if (m_dialogue_box_obj)
		{
			m_dialogue_box_obj->SetOnlyActive(false);
			m_dialogue_box->SetTexture(nullptr);
		}

		//Off the collider button
		if (m_collider_obj[m_currID])
		{
			m_collider_obj[m_currID]->SetOnlyActive(false);
			m_collider_obj[m_currID]->SetOnlyActive(false);
		}

		//Off the next button
		if (m_nextbutton)
			m_nextbutton->SetOnlyActive(false);

		m_currDialogue = 0;
		m_currID++;
		m_dialogueMode = false;

	}

	else
	{
		//Update the current dialogue count
		m_currDialogue++;
		
		//Set the dialogue text to the new texture
		Texture2D * tex = m_dialogues[m_currID][m_currDialogue];
		m_dialogue_box->SetTexture(tex);
	}

}
