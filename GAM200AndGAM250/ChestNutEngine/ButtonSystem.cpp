#include "ButtonSystem.h"

ButtonSystem::ButtonSystem(XBoxInputSystem * xbox, InputSystem * in, SceneActorFactory * fac, EventHandler * ev, SpriteSheetFactory * sprite, bool & pause, float & mul)
	:m_xbox{ xbox }, m_input { in}, m_fac{ fac }, m_ev{ ev }, selected{ 0 }, m_spriteFac{ sprite }, m_currentMenu{ 0 }, m_prevMenu{ 0 }, m_pause{ pause }, m_multiplier{ mul }
{
	m_ev->SubscribeEvent(this, &ButtonSystem::ReInitialize);
	m_volume.reserve(5);
}

ButtonSystem::~ButtonSystem()
{
}

//Load through the scene factory and store all button components that exist
void ButtonSystem::Initialize()
{
	m_currentMenu = 0;
	m_prevMenu = 0;
	selected = 0;
	m_currentSection = 0;

	auto & objs = m_fac->GetSceneActorContainer();
	for (auto & obj : objs)
	{
		auto & comps = obj.second->GetPrefabActor()->GetComponents();
		if (comps.find(ComponentType::ButtonComponent) != comps.end())
		{
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);
			
			unsigned section = button->m_sectionID;
			unsigned id = button->m_buttonID;

			//Initialize buttons;
			if (button->m_icon != -1)
				m_volume[button->m_icon] = obj.second;


			else if (button->m_menuID == m_currentMenu)
			{
				if (button->m_hovered)
				{
					obj.second->SetOnlyActive(false);
					m_hovered_[section][id] = obj.second;
				}

				else
				{
					obj.second->SetOnlyActive(true);
					m_notHovered_[section][id] = obj.second;
					m_buttons_[section][id] = button;
				}
			}

		}

	}

	//Initialize the selected menu
	if (m_hovered_[m_currentSection].size())
	{
		HoverButton(m_currentSection, selected);
	}

	if (m_volume.size())
	{
		for (auto & elem : m_volume)
			elem.second->SetOnlyActive(false);
	}




	/*
	m_volume.clear();

	auto & objs = m_fac->GetSceneActorContainer();
	auto sprites = m_spriteFac->GetContainer();

	m_currentMenu = 0;
	m_prevMenu = 0;

	//Initialize map
	for (auto & obj : objs)
	{
		auto & comps = obj.second->GetPrefabActor()->GetComponents();
		if (comps.find(ComponentType::ButtonComponent) != comps.end()
			&& comps.find(ComponentType::SpriteComponent) != comps.end())
		{
			SpriteComponent * sprite = dynamic_cast<SpriteComponent *>(comps[ComponentType::SpriteComponent]);
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);

			//Initialize buttons;
			if (button->m_icon != -1)
				m_volume[button->m_icon] = obj.second;

			//If the button belongs in the same menu and its not an icon
			else if (button->m_menuID == m_currentMenu)
			{
				obj.second->SetActive(true);
				if (button->m_isMenu == false)
				{
					m_buttons_[button->m_sectionID][button->m_buttonID] = button;
					m_sprites_[button->m_sectionID][button->m_buttonID] = sprite;

					unsigned active = GetSpriteKeyBySpriteName(sprites, m_buttons_[button->m_sectionID][button->m_buttonID]->GetActiveTexName());
					unsigned inactive = GetSpriteKeyBySpriteName(sprites, m_buttons_[button->m_sectionID][button->m_buttonID]->GetInactiveTexName());

					Texture2D * active_tex = (*sprites)[active];
					Texture2D * inactive_tex = (*sprites)[inactive];

					button->SetActiveTex(active_tex);
					button->SetInactiveTex(inactive_tex);
				}
			}
			else
				obj.second->SetActive(false);
		}
	}



	//Initialize the selected menu
	if (m_sprites_[m_currentSection].size())
	{
		selected = 0;
		Texture2D * active_tex = m_buttons_[m_currentSection][selected]->GetActiveTex();
		m_sprites_[m_currentSection][selected]->SetTexture(active_tex);
	}

	if (m_volume.size())
	{
		for (auto & elem : m_volume)
			elem.second->SetOnlyActive(false);
	}
	*/
}



void ButtonSystem::Update()
{
	if (m_buttons_.size())
	{
		if (m_input->InputCheckTriggered(CVK_UP) || m_input->InputCheckTriggered(CVK_W) || m_xbox->isLStickTriggered(1))
		{
			m_currentSection--;

			if (m_currentSection < 0)
				m_currentSection = 0;

			std::cout << "Current section is: " << m_currentSection << std::endl;
			selected = 0;
			HoverButton(m_currentSection, selected);

			m_ev->PublishEvent(new PlayAudio{ Audio{m_buttons_[m_currentSection][selected]->m_navigate, false, 0.1f} });
		}
			
		if (m_input->InputCheckTriggered(CVK_RIGHT) || m_input->InputCheckTriggered(CVK_D))
		{

			selected++;

			if (selected == m_buttons_[m_currentSection].size())
				selected = (int)m_buttons_[m_currentSection].size() - 1;

			std::cout << "Current selected is: " << selected << std::endl;

			HoverButton(m_currentSection, selected);

			m_ev->PublishEvent(new PlayAudio{ Audio{m_buttons_[m_currentSection][selected]->m_navigate, false, 0.5f} });
		}

		if (m_input->InputCheckTriggered(CVK_DOWN) || m_input->InputCheckTriggered(CVK_S) || m_xbox->isLStickTriggered(2))
		{
			m_currentSection++;

			if (m_currentSection == m_buttons_.size())
				m_currentSection = (int)m_buttons_.size() - 1;

			std::cout << "Current section is: " << m_currentSection << std::endl;
			selected = 0;

			HoverButton(m_currentSection, selected);

			m_ev->PublishEvent(new PlayAudio{ Audio{m_buttons_[m_currentSection][selected]->m_navigate, false, 0.5f} });
		}


		if (m_input->InputCheckTriggered(CVK_LEFT) || m_input->InputCheckTriggered(CVK_A) )
		{
			selected--;

			if (selected < 0)
				selected = 0;

			std::cout << "Current selected is: " << selected << std::endl;

			HoverButton(m_currentSection, selected);

			m_ev->PublishEvent(new PlayAudio{ Audio{m_buttons_[m_currentSection][selected]->m_navigate, false, 0.5f} });

		}

		if (m_input->InputCheckTriggered(CVK_ENTER) || m_input->InputCheckTriggered(CVK_SPACE) || m_xbox->isKeyTriggered(BUTTON_A))
		{
			float timer = 0.0f;
			m_ev->PublishEvent(new PlayAudio{ Audio{m_buttons_[m_currentSection][selected]->m_enter,  false, 0.5f} });
			while (timer >= 300.f)
				timer += 1 / 60.0f;
			ActivateButton(m_currentSection, selected);
		}

		if (m_volume.size() == 5)
		{
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(m_volume[1]->GetComponent(ComponentType::ButtonComponent));
			if (button)
			{
				int menu = button->m_menuID;
				if (menu == m_currentMenu)
					UpdateVolumeIcon();
			}
		}


	}

}


void ButtonSystem::ClearSelection()
{
	
	for (unsigned j = 0; j < m_buttons_.size(); ++j)
	{
		for (unsigned i = 0; i < m_buttons_[j].size(); ++i)
		{
			if(m_hovered_[j][i])
				m_hovered_[j][i]->SetOnlyActive(false);
			if (m_notHovered_[j][i])
				m_notHovered_[j][i]->SetOnlyActive(true);
		}
	}

	m_currentSection = 0;
	selected = 0;
}


void ButtonSystem::ReloadMenu(unsigned section, unsigned index)
{
	ButtonComponent * old_button = m_buttons_[section][index];
	auto & objs = m_fac->GetSceneActorContainer();

	//Set the current menu objects to inactive
	for (auto & obj : objs)
	{
		auto & comps = obj.second->GetPrefabActor()->GetComponents();
		if (comps.find(ComponentType::ButtonComponent) != comps.end())
		{
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);

			if (button->m_menuID == m_currentMenu) //If the menu matches, set the object to inactive
				obj.second->SetActive(false);
		}
	}



	if (old_button && old_button->m_next != -1)
	{
		//update the new menu id
		m_prevMenu = m_currentMenu;
		m_currentMenu = old_button->m_next;

		//empty the sprite and button
		m_buttons_.clear();
		m_hovered_.clear();
		m_notHovered_.clear();

		//reload the containers with the new menu buttons
		for (auto & obj : objs)
		{
			auto & comps = obj.second->GetPrefabActor()->GetComponents();
			if (comps.find(ComponentType::ButtonComponent) != comps.end()
				&& comps.find(ComponentType::SpriteComponent) != comps.end())
			{
				ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);

				if (button->m_menuID == m_currentMenu && button->m_icon == -1)// if the menu id of the buttons matches
				{
					//set the object to active
					obj.second->SetActive(true);

					if (button->m_isMenu == false) //load the button and sprite if its not a menu
					{
						m_buttons_[button->m_sectionID][button->m_buttonID] = button;
						if(button->m_hovered)
							m_hovered_[button->m_sectionID][button->m_buttonID] = obj.second;
						else
							m_notHovered_[button->m_sectionID][button->m_buttonID] = obj.second;
					}
				}
			}
		}

	}

	//reinitialize the select to zero
	HoverButton(0, 0);
}

void ButtonSystem::ActivateButton(unsigned section, unsigned index)
{
	
	ButtonComponent * button = m_buttons_[section][index];
	if (button)
	{
		switch (button->m_type)
		{
		case(GAMESTATE):
			m_ev->PublishEvent(new LoadGameState{ button->GetGameStatePath() });
			break;

		
		case(CHANGEMENU): ReloadMenu(section, index); break;


		case(COLLAPSEMENU): CollapseMenu();  break;
		
		case(SCRIPT): break;

		case(RESUMEMENU):
			DeactivateMenu(PAUSE);
			m_pause = false;
			break;

		case(EXIT):
			m_ev->PublishEvent(new QuitGame{});

		case(INCREASEVOL):
				m_multiplier = m_multiplier == 1.0f ? 1.0f : m_multiplier += 0.2f;
				std::cout << "Volume is " << m_multiplier << std::endl;
				break;
	
		case(DECREASEVOL):
				m_multiplier = m_multiplier <= 0.1f ? 0.0f : m_multiplier -= 0.2f;
				std::cout << "Volume is " << m_multiplier << std::endl;
				break;

		case(PLAYERCONTINUE):
				DeactivateMenu(PLAYERLOSE);
				m_pause = false;
				m_ev->PublishEvent(new RestartLevel{});
				std::cout << "RESTART";
		}
	}
}

void ButtonSystem::ActivateMenu(int index)
{
	m_buttons_.clear();
	m_hovered_.clear();
	m_notHovered_.clear();

	m_prevMenu = m_currentMenu;
	m_currentMenu = index;
	selected = 0;

	auto & objs = m_fac->GetSceneActorContainer();
	for (auto & obj : objs)
	{
		auto & comps = obj.second->GetPrefabActor()->GetComponents();
		if (comps.find(ComponentType::ButtonComponent) != comps.end())
		{
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);

			unsigned section = button->m_sectionID;
			unsigned id = button->m_buttonID;

			//Initialize buttons;
			if (button->m_icon != -1)
				m_volume[button->m_icon] = obj.second;


			else if (button->m_menuID == index)
			{
				if (button->m_hovered)
				{
					obj.second->SetOnlyActive(false);
					m_hovered_[section][id] = obj.second;
				}

				else
				{
					obj.second->SetOnlyActive(true);
					m_notHovered_[section][id] = obj.second;
					m_buttons_[section][id] = button;
				}
			}

		}

	}

	//Initialize the selected menu
	if (m_hovered_[m_currentSection].size())
		HoverButton(m_currentSection, selected);
}


void ButtonSystem::DeactivateMenu(int menuID)
{
	
	auto & objs = m_fac->GetSceneActorContainer();

	//Set the current menu objects to inactive
	for (auto & obj : objs)
	{
		auto & comps = obj.second->GetPrefabActor()->GetComponents();
		if (comps.find(ComponentType::ButtonComponent) != comps.end())
		{
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);
			
			//If the menu matches, set the object to inactive
			if (button->m_menuID == menuID)
			{	
				obj.second->SetOnlyActive(false);
			}
		}
	}

	m_buttons_.clear();
	m_hovered_.clear();
	m_notHovered_.clear();
	selected = 0;
}

void ButtonSystem::TogglePauseMenu(bool pause)
{

	auto & objs = m_fac->GetSceneActorContainer();
	int count = 0;

	for (auto & obj : objs)
	{
		auto & comps = obj.second->GetPrefabActor()->GetComponents();
		if (comps.find(ComponentType::ButtonComponent) != comps.end())
		{
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);
			//If the menu matches, set the object to active
			if (button->m_menuID == PAUSEMENU)
				count++;
		}
	}

	if (count)
	{
		m_buttons_.clear();
		m_hovered_.clear();
		m_notHovered_.clear();
		selected = 0;

		if (pause)
		{
			std::cout << "Pause menu activated" << std::endl;
			//Set the current menu objects to active
			for (auto & obj : objs)
			{
				auto & comps = obj.second->GetPrefabActor()->GetComponents();
				if (comps.find(ComponentType::ButtonComponent) != comps.end())
				{
					ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);
	
					//If the menu matches, set the object to active
					if (button->m_menuID == PAUSEMENU)
					{
						obj.second->SetActive(true);

						if (button->m_isMenu == false)
						{
							int ID = button->m_buttonID;
							int section = button->m_sectionID;
							m_buttons_[section][ID] = button;

							if (button->m_hovered)
								m_hovered_[section][ID] = obj.second;
							else
								m_notHovered_[section][ID] = obj.second;

							m_currentMenu = PAUSEMENU;
						}
					}
				}
			}

			HoverButton(0, 0);
		}

		else
		{
			std::cout << "Pause menu de-activated" << std::endl;
			DeactivateMenu(m_currentMenu);
		}
	}
}

void ButtonSystem::ActivateDeathMenu()
{
	auto & objs = m_fac->GetSceneActorContainer();
	int count = 0;

	for (auto & obj : objs)
	{
		auto & comps = obj.second->GetPrefabActor()->GetComponents();
		if (comps.find(ComponentType::ButtonComponent) != comps.end())
		{
			ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);
			//If the menu matches, set the object to active
			if (button->m_menuID == PAUSEMENU)
				count++;
		}
	}

	if (count)
	{
		m_buttons_.clear();
		m_hovered_.clear();
		m_notHovered_.clear();
		selected = 0;

		//Set the current menu objects to active
		for (auto & obj : objs)
		{
			auto & comps = obj.second->GetPrefabActor()->GetComponents();
			if (comps.find(ComponentType::ButtonComponent) != comps.end())
			{
				ButtonComponent * button = dynamic_cast<ButtonComponent *>(comps[ComponentType::ButtonComponent]);

				//If the menu matches, set the object to active
				if (button->m_menuID == PLAYERLOSE)
				{
					obj.second->SetActive(true);

					if (button->m_isMenu == false)
					{
						int ID = button->m_buttonID;
						int section = button->m_sectionID;
						m_buttons_[section][ID] = button;

						if (button->m_hovered)
							m_hovered_[section][ID] = obj.second;
						else
							m_notHovered_[section][ID] = obj.second;

						m_currentMenu = PLAYERLOSE;
					}
				}
			}
		}

		HoverButton(0, 0);
	}
}


void ButtonSystem::UpdateVolumeIcon()
{
	if (m_volume.size() == 0)
		return;

	//Clear the volume
	for (auto & elem : m_volume)
		elem.second->SetActive(false);

	if (m_multiplier == 0.0f)
		return;

	if(m_multiplier >= 0.2f)
		m_volume[1]->SetActive(true);
	if (m_multiplier >= 0.4f)
		m_volume[2]->SetActive(true);
	if (m_multiplier >= 0.6f)
		m_volume[3]->SetActive(true);
	if (m_multiplier >= 0.8f)
		m_volume[4]->SetActive(true);
	if (m_multiplier >= 1.f)
		m_volume[5]->SetActive(true);

}

void ButtonSystem::CollapseMenu()
{
	
	std::cout << "Current menu is " << m_currentMenu << std::endl;
	std::cout << "Previous menu is " << m_prevMenu << std::endl;
	int newMenu = m_prevMenu;
	DeactivateMenu(m_currentMenu);
	std::cout << "Died at activate" << std::endl;
	ActivateMenu(newMenu);
}


void ButtonSystem::HoverButton(unsigned section, unsigned id)
{
	
	std::cout << "Current menu is " << m_currentMenu << std::endl;
	ClearSelection();

	if (m_buttons_[section].size())
	{
		if(m_hovered_[section][id])
			m_hovered_[section][id]->SetOnlyActive(true);

		if(m_notHovered_[section][id])
			m_notHovered_[section][id]->SetOnlyActive(false);

		selected = id;
		m_currentSection = section;
	}
}

void ButtonSystem::ReInitialize(LoadLevel *)
{
	m_buttons_.clear();
	m_hovered_.clear();
	m_volume.clear();
	Initialize();
}
