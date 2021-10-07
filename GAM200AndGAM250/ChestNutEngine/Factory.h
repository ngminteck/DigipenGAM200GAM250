/******************************************************************************/
/*!
\file   Factory.h

\author ONG, Zhi Hao Jaryl(100%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the prototypes for the Factory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "GameObject.h"
#include "ComponentFactory.h"
#include <vector>
#include <string>
#include <map>

#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>
#include <filesystem>

#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"


#include "SpriteFactory.h"


class Factory
{
public:
	Factory();
	~Factory();

	bool Initialize();

	Component* InitComponent(std::string component, const rapidjson::Value& node);

	void ScanObjectFile();
	void CreateObject(const char * filename);
	void CreateInstanceObject(const char * filename);

	std::unordered_map<unsigned, GameObject *> & GetGameObjectsInstance() ;
	std::unordered_map<unsigned, GameObject *> & GetPrefabs() ;


	std::unordered_map<unsigned, GameObject *> * CloneObjects() const;
	std::unordered_map<std::string, ComponentFactory*> GetFactories() const;

	SpriteFactory* GetSpriteFactoryInfo() const;

	void ClearGameInstance();
	void ClearAllSelection();

	void SaveLevel(const std::string &);
	void SavePrefab();
	void CreateGameObject();
	void CreatePrefab(int object_type);
	void CloneObject(GameObject * obj);
	void DeleteObject(unsigned);
	
private:
	unsigned m_prefabs_id, m_gameObjsInstance_id;
	std::unordered_map<std::string, ComponentFactory*> m_objMap; //Map that binds std::string to distributed factories
	std::unordered_map<std::string, ResourceFactory*>  m_ResourceFactoryMap;
	//ID Name     Object Info
	std::unordered_map<unsigned, GameObject *> m_prefabs; //For archetype
	std::unordered_map<unsigned, GameObject *> m_gameObjsInstance; //For level editor

	SpriteFactory * spritefactory;

	GameObject * CreateInstanceChildObject(const rapidjson::Value& node, GameObject * parent_obj);


};
