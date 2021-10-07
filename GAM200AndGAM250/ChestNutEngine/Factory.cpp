/******************************************************************************/
/*!
\file   Factory.cpp

\author ONG, Zhi Hao Jaryl(60%), NG, Min Teck (40%)
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains the functions for the Factory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Factory.h"
#include "GameObject.h"
#include "HPFactory.h"
#include "RigidBodyFactory.h"
#include "SpriteFactory.h"
#include "GravityFactory.h"
#include "CollisionFactory.h"
#include "ControlFactory.h"
#include "AABBFactory.h"
#include "TransformFactory.h"
#include "MovementFactory.h"
#include "TriangleFactory.h"
#include "HotspotFactory.h"
#include "ShaderFactory.h"
#include "FontFactory.h"


#include <random>


namespace FactoryUtils
{
	static int OBJECT_COUNT = 0;
}

Factory::Factory()
:m_prefabs_id{ 0 }, m_gameObjsInstance_id{ 1 }
{
	//Initialize map
	m_objMap["HPComponent"]			= new HPFactory{};
	m_objMap["RigidBodyComponent"]	= new RigidBodyFactory{};
	m_objMap["SpriteComponent"]		= new SpriteFactory{};
	m_objMap["GravityComponent"]	= new GravityFactory{};
	m_objMap["ControlComponent"]	= new ControlFactory{};
	m_objMap["AABBComponent"]		= new AABBFactory{};
	m_objMap["TriangleComponent"]   = new TriangleFactory{};
	m_objMap["HotspotComponent"]    = new HotspotFactory{};
	m_objMap["CollisionComponent"]	= new CollisionFactory{};
	m_objMap["TransformComponent"]	= new TransformFactory{};
	m_objMap["MovementComponent"]	= new MovementFactory{};
	
	//m_ResourceFactoryMap["Font"]	= new FontFactory{};
}


Factory::~Factory()
{
	for (auto & elem : m_gameObjsInstance) //Free the game objects
	{
		delete elem.second;
	}


	for (auto elem = m_prefabs.begin(); elem != m_prefabs.end(); ++elem) //Free the game objects
	{
		delete elem->second;
	}

	for (auto & elem : m_objMap) //Free the pointers to the object creators
	{
		delete elem.second;
	}
}

bool Factory::Initialize()
{
	if (SpriteFactory* spritefac = dynamic_cast<SpriteFactory*>(m_objMap["SpriteComponent"]))
	{
		spritefac->Initialize();
		spritefactory = spritefac;
	}
	//if (FontFactory* fontfac = dynamic_cast<FontFactory*>(m_ResourceFactoryMap["Font"]))
	//	fontfac->LoadFromFile("Resource/Json/ResourceData/font.json");

	CreateObject("Resource/Json/Prefab/Default.json");
	return true;
}

bool CheckUniqueName(const std::unordered_map<unsigned, GameObject *> & container, std::string search)
{
	std::string search_id = search;
	std::transform(search_id.begin(), search_id.end(), search_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container.begin(); it != container.end(); ++it)
	{
		std::string container_name_id = it->second->GetIDName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_id.compare(container_name_id) == 0)
			return false;

	}

	return true;
}

unsigned GetMapKeyByName(const std::unordered_map<unsigned, GameObject *> & container, std::string search)
{
	std::string search_id = search;
	std::transform(search_id.begin(), search_id.end(), search_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container.begin(); it != container.end(); ++it)
	{
		std::string container_name_id = it->second->GetIDName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_id.compare(container_name_id) == 0)
			return it->first;

	}
	return 0;
}

SpriteFactory * Factory::GetSpriteFactoryInfo() const
{
	return spritefactory;
}

std::unordered_map<std::string, ComponentFactory *> Factory::GetFactories() const
{
	return m_objMap;
}

Component* Factory::InitComponent(std::string component, const rapidjson::Value& node)
{
	std::unordered_map<std::string, ComponentFactory*>::const_iterator iter = m_objMap.find(component);
	return (iter != m_objMap.end()) ? (iter->second)->CreateComponent(node) : 0;
}

std::unordered_map<unsigned, GameObject *> & Factory::GetPrefabs() 
{
	return m_prefabs;
}

std::unordered_map<unsigned, GameObject *> & Factory::GetGameObjectsInstance() 
{
	return m_gameObjsInstance;
}



void Factory::CreateObject(const char * filename)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filename, "r");
	std::cout << filename << std::endl;

	if (err == 0)
	{
		char readBuffer[65536]; // assign the buffersize
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); // rapidjson file reader stream
		rapidjson::Document document; // rapidjson document for using rpaidjson document format
		document.ParseStream(is); // pass in the file stream into the document
		
		// if json file type is prefab then process , if not don't do anything and close
		std::string json_file_type = document["JsonFileType"].GetString();
	
		if (json_file_type.compare("Prefab") == 0)
		{
			
			GameObject * obj = new GameObject(static_cast<ObjectType>(document["Object"]["m_ObjType"].GetInt()));
			// this read the open file, instead value from json
			obj->SetPath(filename);

			if (document["Object"].HasMember("Component"))
			{
				const rapidjson::Value& compObj = document["Object"]["Component"]; // access the object's component array container

				for (rapidjson::SizeType i = 0; i < compObj.Size(); i++) // loop throught all component element in component array container
				{
					std::string s = (compObj[i]["ComponentType"].GetString()); // get the component type
					//std::cout << s << std::endl;
					Component * comp = InitComponent(s, compObj[i]); // call the init component function
					obj->AddComponent(comp);// add the component to the object
				}
			}

			const std::string name_id = document["Object"]["m_ObjIDName"].GetString();
			std::stringstream find_id;
			find_id.str(std::string());
			find_id << name_id;
			unsigned no = 1;

			bool unique = CheckUniqueName(m_prefabs, find_id.str());

			while (!unique)
			{
				++no;
				find_id.str(std::string());
				find_id << name_id << "(" << no << ")";
				unique = CheckUniqueName(m_prefabs, find_id.str());
			}

			obj->SetIDName(find_id.str());
			obj->SetArcheType(obj->GetIDName());

			m_prefabs.emplace(m_prefabs_id, obj);
			
			++m_prefabs_id;
		}
		fclose(fp);
	}
	else
	{
		std::cout << "Fail to open " << filename << " file" << std::endl;
	}
}

void Factory::CreateInstanceObject(const char * filename)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filename, "r");
	if (err == 0)
	{
		//std::cout << "Loading content of " << filename << " file" << std::endl;
		char readBuffer[65536]; // assign the buffersize
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); // rapidjson file reader stream
		rapidjson::Document document; // rapidjson document for using rpaidjson document format
		document.ParseStream(is); // pass in the file stream into the document
		std::string json_file_type = document["JsonFileType"].GetString();
		if (json_file_type.compare("Level") == 0)
		{
			const rapidjson::Value& resourceRequiredList = document["Resource"]; // access the level resource prefab needed for the level
			for (rapidjson::SizeType i = 0; i < resourceRequiredList.Size(); i++)  // get all the element inside the object label array
			{
				if(CheckUniqueName(m_prefabs, resourceRequiredList[i]["m_archeType"].GetString()))
				{
					std::string mastercopy_filename = resourceRequiredList[i]["m_ObjPath"].GetString();
					CreateObject(mastercopy_filename.c_str());
				}
			}
			const rapidjson::Value& levelList = document["Level"]; // access the level item


			for (rapidjson::SizeType a = 0; a < levelList.Size(); a++)
			{
				std::string name_id_lookup = levelList[a]["m_archeType"].GetString();
				unsigned result_id = GetMapKeyByName(m_prefabs, name_id_lookup);

				if (result_id == 0)
					std::cout << "cannot find \"" << name_id_lookup << "\" prefab, it will use a default prefab instead." << std::endl;

				GameObject * obj = new GameObject(*(m_prefabs.at(result_id)));

				obj->SetIDName(name_id_lookup.c_str());

				std::string name_id = obj->GetIDName();
				std::stringstream find_id;
				find_id.str(std::string());
				find_id << name_id;
				unsigned no = 1;

				bool unique = CheckUniqueName(m_gameObjsInstance, find_id.str());

				while (!unique)
				{
					++no;
					find_id.str(std::string());
					find_id << name_id << "(" << no << ")";
					unique = CheckUniqueName(m_gameObjsInstance, find_id.str());
				}

				obj->SetIDName(find_id.str());
				auto comps = obj->GetComponentsPtr();
				// some stats edit
				if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
				{
					RigidBodyComponent* rbComp = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
				

					rbComp->SetPosition(Vector2D(levelList[a]["m_pos.x"].GetFloat(), levelList[a]["m_pos.y"].GetFloat()));
					
				}

				if (comps->find(ComponentType::TransformComponent) != comps->end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
					trans->SetScale(Vector3D(levelList[a]["m_Scale.x"].GetFloat(), levelList[a]["m_Scale.y"].GetFloat(), 0));

				}

				if (comps->find(ComponentType::AABBComponent) != comps->end())
				{
					AABBComponent* aabb = dynamic_cast<AABBComponent*>(comps->find(ComponentType::AABBComponent)->second);
					aabb->SetAABBScale(Vector2D(levelList[a]["m_aabbScale.x"].GetFloat(), levelList[a]["m_aabbScale.y"].GetFloat()));
				}

				if (levelList[a].HasMember("m_ChildObj"))
				{
					const rapidjson::Value& levelChildObjectList = levelList[a]["m_ChildObj"]; // access the level item
					for (rapidjson::SizeType k = 0; k < levelChildObjectList.Size(); k++)  // get all the level item
					{
						
					}
				}
				m_gameObjsInstance.emplace(m_gameObjsInstance_id, obj);
				++m_gameObjsInstance_id;
			}
			

		}

		fclose(fp);
	}
	else
	{
		std::cout << "Fail to open " << filename << " file" << std::endl;
	}
}

GameObject * Factory::CreateInstanceChildObject(const rapidjson::Value& node, GameObject * parent_obj)
{
	std::string name_id_lookup = node["ObjectName"].GetString();
	unsigned result_id = GetMapKeyByName(m_prefabs, name_id_lookup);

	if (result_id == 0)
		std::cout << "cannot find \"" << name_id_lookup << "\" prefab, it will use a default prefab instead." << std::endl;

	GameObject * obj = new GameObject(*(m_prefabs.at(result_id)));

	obj->SetParentObject(parent_obj);
	obj->SetArcheType(name_id_lookup.c_str());
	obj->SetIDName(name_id_lookup.c_str());

	if (node.HasMember("m_ChildObj"))
	{
		const rapidjson::Value& levelChildObjectList =node["m_ChildObj"]; // access the level item
		for (rapidjson::SizeType k = 0; k < levelChildObjectList.Size(); k++)  // get all the level item
		{
		
		}
	}

	return obj;

}

std::unordered_map <unsigned, GameObject *> * Factory::CloneObjects() const
{
	auto * map = new std::unordered_map<unsigned, GameObject*>;
	auto begin = m_prefabs.begin();
	auto end = m_prefabs.end();
	
	//Deep copy
	for (auto itor = begin; itor != end; ++itor)
	{
		map->insert(std::make_pair(itor->first, new GameObject{*(itor)->second}));
	}
	return map;
}

void Factory::SaveLevel(const std::string & levelName)
{
	std::string pathName = "Resource/Json/Level/";
	//Initialize archetypes
	std::unordered_map<std::string, std::string> m_resource;
	for (auto & elem : m_gameObjsInstance)
	{
		m_resource[elem.second->GetArcheType()] = elem.second->GetPath();
	}

	rapidjson::StringBuffer s;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

	writer.SetMaxDecimalPlaces(1);

	writer.StartObject();
	writer.Key("JsonFileType");
	writer.String("Level");
	writer.Key("Resource");
	writer.StartArray();
	for (auto & elem : m_resource)
	{
		writer.StartObject();
		writer.Key("m_archeType");
		writer.String(elem.first.c_str());
		writer.Key("m_ObjPath");
		writer.String(elem.second.c_str());
		writer.EndObject();
	}
	writer.EndArray();
	writer.Key("Level");
	writer.StartArray();
	for (const auto & elem2 : m_gameObjsInstance)
	{
	
		writer.StartObject();
		writer.Key("m_archeType");
		writer.String(elem2.second->GetArcheType().c_str());

		if (elem2.second->GetComponents().size() > 0)
		{
			auto  map = elem2.second->GetComponents();
			// small hack temp , should check what different with prefab and then will save the stuff
			if( map.find(ComponentType::RigidBodyComponent) != map.end())
			{
				RigidBodyComponent * rbcomp = dynamic_cast<RigidBodyComponent*>(map[ComponentType::RigidBodyComponent]);
				writer.Key("m_pos.x");
				writer.Double(rbcomp->GetPosition().x);
				writer.Key("m_pos.y");
				writer.Double(rbcomp->GetPosition().y);
			}

			if (map.find(ComponentType::TransformComponent) != map.end())
			{
				TransformComponent * trans = dynamic_cast<TransformComponent*>(map[ComponentType::TransformComponent]);
				writer.Key("m_Scale.x");
				writer.Double(trans->GetScale().x);
				writer.Key("m_Scale.y");
				writer.Double(trans->GetScale().y);
				
			}

			if (map.find(ComponentType::AABBComponent) != map.end())
			{
				AABBComponent * aabb = dynamic_cast<AABBComponent*>(map[ComponentType::AABBComponent]);
				writer.Key("m_aabbScale.x");
				writer.Double(aabb->GetAABBScale().x);
				writer.Key("m_aabbScale.y");
				writer.Double(aabb->GetAABBScale().y);
			}
		}

		/*possible child object in instance*/
		//if (elem2.second->GetChildObjects().size() > 0)
		//{

		//}

		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();

	std::ofstream of;
	std::string fullName;
	fullName += pathName + levelName + ".json";

	of.open(fullName.c_str());
	of << s.GetString();
	if (!of.good())
		throw std::runtime_error("Failed to save the Level!");
}

void Factory::SavePrefab() 
{
	for (auto & elem : m_prefabs)
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

		std::stringstream file_name;
		file_name.str(std::string());
		file_name << "Resource/Json/TestSavePrefab/" << elem.second->GetIDName().c_str() << ".json";

		writer.SetMaxDecimalPlaces(6);

		writer.StartObject();
		writer.Key("JsonFileType");
		writer.String("Prefab");
		writer.Key("File");
		writer.String(file_name.str().c_str());
		writer.Key("Object");

		writer.StartObject();

		writer.Key("m_ObjType");
		writer.Int(elem.second->GetType());

		writer.Key("m_ObjIDName");
		writer.String(elem.second->GetIDName().c_str());

		if (elem.second->GetComponents().size() > 0)
		{
			writer.Key("Component");
			writer.StartArray();

			for (auto & elem2 : elem.second->GetComponents())
			{
				writer.StartObject();

				elem2.second->Unserialize(writer);

				writer.EndObject();
			}

			writer.EndArray();
		}


		writer.EndObject();
		writer.EndObject();

		std::ofstream of;
		of.open(file_name.str().c_str());
		of << s.GetString();
		if (!of.good())
			throw std::runtime_error("Failed to save the prefab!");
	}
}

void Factory::CreateGameObject()
{
	for (auto it = m_prefabs.begin(); it != m_prefabs.end(); ++it)
	{
		if (it->second->GetSelection() == true)
		{
			GameObject * obj = new GameObject(*(m_prefabs.at(it->first)));
			std::string name_id = obj->GetIDName();
			std::stringstream find_id;
			find_id.str(std::string());
			find_id << name_id;
			unsigned no = 1;

			bool unique = CheckUniqueName(m_gameObjsInstance, find_id.str());

			while (!unique)
			{
				++no;
				find_id.str(std::string());
				find_id << name_id << "(" << no << ")";
				unique = CheckUniqueName(m_gameObjsInstance, find_id.str());
			}

			obj->SetIDName(find_id.str());

			m_gameObjsInstance.emplace(m_gameObjsInstance_id,obj);
			++ m_gameObjsInstance_id;
		}
	}
	//GameObject * obj = new GameObject{ ++m_gameObjsInstance_id };
	//obj->AddComponent(new AABBComponent{});
	//obj->AddComponent(new RigidBodyComponent{});

	//m_gameObjsInstance[m_gameObjsInstance_id] = obj;
}


void Factory::CloneObject(GameObject * obj)
{
	std::string name = obj->GetIDName() + " Copy";
	GameObject * input = new GameObject{ *obj };
	obj->SetIDName(name);
	m_gameObjsInstance.insert(std::make_pair(++m_gameObjsInstance_id, input));
}

void Factory::DeleteObject(unsigned id)
{
	for (auto it = m_gameObjsInstance.begin(); it != m_gameObjsInstance.end(); ++it)
	{
		if (it->first == id)
		{
			delete it->second;
			it = m_gameObjsInstance.erase(it);
			break;
		}
	}
}

void Factory::ClearGameInstance()
{
	for (auto & elem : m_gameObjsInstance) 
	{
		delete elem.second;
	}
	m_gameObjsInstance.clear();
}


void Factory::ClearAllSelection()
{
	for (auto & elem : m_gameObjsInstance)
	{
		elem.second->ClearSelection();
	}
}

