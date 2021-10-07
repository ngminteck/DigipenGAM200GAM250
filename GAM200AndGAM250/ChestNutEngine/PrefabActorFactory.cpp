/******************************************************************************/
/*!
\file   PrefabActorFactory.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	the file contain the main prefab container, loading and saving, checking
	prefab integrity and the function of editing the prefab in editor, 
	like select,move,duplicate,rename,delete,new

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "PrefabActorFactory.h"

PrefabActorFactory::PrefabActorFactory(SpriteSheetFactory* spritesheet_fac, sol::state* luastate, MainWindow *win, GraphicsSystem* graphic)
	:prefab_actor_container_key{ 0 }, m_SpriteSheetFactory{ spritesheet_fac }, LuaState{ luastate }, m_win{ win }, m_graphic{ graphic },
	digipen_logo{ nullptr }, team_logo{ nullptr }
{
	ObjectTypeList.emplace(1, "Player");
	ObjectTypeList.emplace(2, "Enemy");
	ObjectTypeList.emplace(3, "Static_Platform");
	ObjectTypeList.emplace(4, "Moving_Platform");
	ObjectTypeList.emplace(5, "Crumbling_Platform");
	ObjectTypeList.emplace(6, "Oneway_Platform");
	ObjectTypeList.emplace(7, "Pit_Traps");
	ObjectTypeList.emplace(8, "Timed_Platform");
	ObjectTypeList.emplace(9, "Tornado_Platform");
	ObjectTypeList.emplace(10, "Background");
	ObjectTypeList.emplace(11, "Invisible_Wall");
	ObjectTypeList.emplace(12, "Death_Zone");
	ObjectTypeList.emplace(13, "Trigger");
	ObjectTypeList.emplace(14, "Switch");
	ObjectTypeList.emplace(15, "Gate");
	ObjectTypeList.emplace(16, "New Button");
	ObjectTypeList.emplace(17, "BackgroundWithMusic");

	ComponentTypeList.emplace(ComponentType::TransformComponent, "TransformComponent");
	ComponentTypeList.emplace(ComponentType::AABBComponent, "AABBComponent");
	ComponentTypeList.emplace(ComponentType::AudioComponent, "AudioComponent");
	ComponentTypeList.emplace(ComponentType::CollisionComponent, "CollisionComponent");
	ComponentTypeList.emplace(ComponentType::MovementComponent, "MovementComponent");
	ComponentTypeList.emplace(ComponentType::RigidBodyComponent, "RigidBodyComponent");
	ComponentTypeList.emplace(ComponentType::ScriptComponent, "ScriptComponent");
	ComponentTypeList.emplace(ComponentType::SpriteComponent, "SpriteComponent");
	ComponentTypeList.emplace(ComponentType::StatsComponent, "StatsComponent");
	ComponentTypeList.emplace(ComponentType::ButtonComponent, "ButtonComponent");
	ComponentTypeList.emplace(ComponentType::CameraComponent, "CameraComponent");
	ComponentTypeList.emplace(ComponentType::CheckpointComponent, "CheckpointComponent");
	ComponentTypeList.emplace(ComponentType::DialogueComponent, "DialogueComponent");
}

PrefabActorFactory::~PrefabActorFactory()
{
	for (auto & elem : prefab_actor_container)
	{
		delete elem.second;
	}

	delete background_logo;
	delete digipen_logo;
	delete team_logo;

	for (auto & elem : m_objMap)
	{
		delete elem.second;
	}
	ObjectTypeList.clear();
	default_prefab_actor_container.clear();
	ComponentTypeList.clear();
}

void PrefabActorFactory::InitComponentFactoryAndConstructDefaultPrefab()
{
	m_objMap["StatsComponent"] = new StatsFactory{};
	m_objMap["RigidBodyComponent"] = new RigidBodyFactory{};
	m_objMap["SpriteComponent"] = new SpriteFactory{ m_SpriteSheetFactory };
	m_objMap["AABBComponent"] = new AABBFactory{};
	m_objMap["CollisionComponent"] = new CollisionFactory{};
	m_objMap["TransformComponent"] = new TransformFactory{};
	m_objMap["MovementComponent"] = new MovementFactory{};
	m_objMap["ScriptComponent"] = new ScriptFactory{ LuaState };
	m_objMap["AudioComponent"] = new AudioFactory{};
	m_objMap["ButtonComponent"] = new ButtonFactory{ m_SpriteSheetFactory };
	m_objMap["CameraComponent"] = new CameraFactory{};
	m_objMap["CheckpointComponent"] = new CheckpointFactory{};
	m_objMap["DialogueComponent"] = new DialogueFactory{};
	
}

bool PrefabActorFactory::ComponentCheck(PrefabActor * lhs, PrefabActor * rhs)
{
	for (auto & elem : lhs->GetComponents())
	{
		if (rhs->GetComponents().find(elem.first) == rhs->GetComponents().end())
		{
			std::cout << "Cannot find " << ComponentTypeList.find(elem.first)->second << std::endl;
			return false;
		}
		else
		{
			auto rhs_comp_map_second = rhs->GetComponents().find(elem.first)->second;
			bool result = true;
			
			if (elem.first == ComponentType::SpriteComponent)
			{
				SpriteComponent * lhs_sprite_comp = dynamic_cast<SpriteComponent*>(elem.second);
				SpriteComponent * rhs_sprite_comp = dynamic_cast<SpriteComponent*>(rhs_comp_map_second);
				result = CompareData(*lhs_sprite_comp, *rhs_sprite_comp);
				if (!result)
				{
					std::cout << "SpriteComponent mismatch" << std::endl;
					return false;
				}
			}
			else if (elem.first == ComponentType::CollisionComponent)
			{
				CollisionComponent * lhs_col_comp = dynamic_cast<CollisionComponent*>(elem.second);
				CollisionComponent * rhs_col_comp = dynamic_cast<CollisionComponent*>(rhs_comp_map_second);
				result = CompareData(lhs_col_comp, rhs_col_comp);
				if (!result)
				{
					std::cout << "CollisionComponent mismatch" << std::endl;
					return false;
				}
			}
			else if (elem.first == ComponentType::RigidBodyComponent)
			{
				RigidBodyComponent * lhs_rigi_comp = dynamic_cast<RigidBodyComponent*>(elem.second);
				RigidBodyComponent * rhs_rigi_comp = dynamic_cast<RigidBodyComponent*>(rhs_comp_map_second);
				result = CompareData(*lhs_rigi_comp, *rhs_rigi_comp);
				if (!result)
				{
					std::cout << "RigidBodyComponent mismatch" << std::endl;
					return false;
				}
			}
			else if (elem.first == ComponentType::StatsComponent)
			{
				StatsComponent * lhs_stats_comp = dynamic_cast<StatsComponent*>(elem.second);
				StatsComponent * rhs_stats_comp = dynamic_cast<StatsComponent*>(rhs_comp_map_second);
				result = CompareData(*lhs_stats_comp, *rhs_stats_comp);
				if (!result)
				{
					std::cout << "StatsComponent mismatch" << std::endl;
					return false;
				}
			}
			else if (elem.first == ComponentType::AABBComponent)
			{
				AABBComponent * lhs_aabb_comp = dynamic_cast<AABBComponent*>(elem.second);
				AABBComponent * rhs_aabb_comp = dynamic_cast<AABBComponent*>(rhs_comp_map_second);
				result = CompareData(*lhs_aabb_comp, *rhs_aabb_comp);
				if (!result)
				{
					std::cout << "AABBComponent mismatch" << std::endl;
					return false;
				}
			}

			else if (elem.first == ComponentType::ScriptComponent)
			{
				ScriptComponent * lhs_script_comp = dynamic_cast<ScriptComponent*>(elem.second);
				ScriptComponent * rhs_script_comp = dynamic_cast<ScriptComponent*>(rhs_comp_map_second);
				result = CompareData(*lhs_script_comp, *rhs_script_comp);
				if (!result)
				{
					std::cout << "ScriptComponent mismatch" << std::endl;
					return false;
				}
			}
			else if (elem.first == ComponentType::AudioComponent)
			{
				AudioComponent * lhs_audio_comp = dynamic_cast<AudioComponent*>(elem.second);
				AudioComponent * rhs_audio_comp = dynamic_cast<AudioComponent*>(rhs_comp_map_second);
				result = CompareData(*lhs_audio_comp, *rhs_audio_comp);
				if (!result)
					return false;
			}
			else
			{
			
			}
		}
	}

	return true;
}

bool PrefabActorFactory::PrefabCheck(PrefabActor * lhs, PrefabActor * rhs, bool diff_prefab_name)
{
	std::cout << "compare with" << lhs->GetPrefabActorName() << std::endl;
	// check path key
	if (!diff_prefab_name)
	{
		if (lhs->GetPath().compare(rhs->GetPath()) != 0)
		{
			std::cout << "path mismatch" << std::endl;
			return false;
		}
	}

	// check all the container size
	if (lhs->GetComponents().size() != rhs->GetComponents().size())
	{
		std::cout << "components size mismatch" << std::endl;
		return false;
	}

	if (lhs->GetChildPrefabActors().size() != rhs->GetChildPrefabActors().size())
	{
		std::cout << "child size mismatch" << std::endl;
		return false;
	}

	if (lhs->GetPrefabActorTagging().size() != rhs->GetPrefabActorTagging().size())
	{
		std::cout << "tagging size mismatch" << std::endl;
		return false;
	}

	// check tagging in detail , object type and prefab name included
	if (lhs->GetPrefabActorTagging().size() > 0)
	{
		for (auto & elem : lhs->GetPrefabActorTagging())
		{
			if (diff_prefab_name && elem.first.compare("prefab_name_key") == 0)
			{
				// skip prefab name check
				continue;
			}

			if (diff_prefab_name && elem.first.compare("object_type_key") == 0)
			{
				// skip object type check
				
				continue;
			}
			
			if (rhs->GetPrefabActorTagging().find(elem.first) == rhs->GetPrefabActorTagging().end())
			{
				std::cout << "missing " << elem.first << " key";
				return false;
			}
			else
			{
				if (elem.second.compare(rhs->GetPrefabActorTagging().find(elem.first)->second) != 0)
				{
					std::cout << elem.first << " key" << " not tally. LHS " << elem.second << " RHS " << rhs->GetPrefabActorTagging().find(elem.first)->second << std::endl;
					return false;
				}
			}
		}
	}

	// check component data in detail
	bool comp_check_result = true;

	comp_check_result = ComponentCheck(lhs, rhs);

	if (!comp_check_result)
	{
		return false;
	}

	// check the child and is recurrsive
	if (lhs->GetChildPrefabActors().size() > 0)
	{
		for (auto & elem : lhs->GetChildPrefabActors())
		{
			unsigned key = GetPrefabKeyByName(rhs->GetChildPrefabActorsPtr(), elem.second->GetPrefabActorName());
			if (key == 0)
			{
				std::cout << "can't found any matching prefab for child" << std::endl;
				return false;
			}
			auto rhs_second_info = rhs->GetChildPrefabActors().find(key)->second;

			if (default_prefab_actor_container.find(rhs_second_info->GetPrefabActorName()) != default_prefab_actor_container.end())
			{
				std::cout << "can't found any matching prefab for child" << std::endl;
				return false;
			}

			bool child_check_result = true;
			child_check_result = PrefabCheck(rhs_second_info, (elem.second));

			if (!child_check_result)
			{
				std::cout << "child mismatch" << std::endl;
				return false;
			}
		}
	}

	return true;
}

unsigned  PrefabActorFactory::FindAnySimilarPrefabData(PrefabActor * prefab_actor)
{
	unsigned result = 0;
	for (auto &elem : prefab_actor_container)
	{
		if (elem.first == 0)
		{
			// skip the default prefab to check
			continue;
		}
		bool check_result = false;

		check_result = PrefabCheck(elem.second, prefab_actor, true);

		// found similar prefab data
		if (check_result)
		{
			result = elem.first;
			break;
		}
	}
	// if 0 mean no any similar 
	return result;
}

std::unordered_map<std::string, ComponentFactory *> PrefabActorFactory::GetFactories() const
{
	return m_objMap;
}


Component* PrefabActorFactory::InitComponent(std::string component, const rapidjson::Value& node)
{
	std::unordered_map<std::string, ComponentFactory*>::const_iterator iter = m_objMap.find(component);
	return (iter != m_objMap.end()) ? (iter->second)->CreateComponent(node) : 0;
}

Component* PrefabActorFactory::InitDefaultComponent(std::string component)
{
	std::unordered_map<std::string, ComponentFactory*>::const_iterator iter = m_objMap.find(component);
	return (iter != m_objMap.end()) ? (iter->second)->CreateComponent() : 0;
}

std::unordered_map<unsigned, PrefabActor *> & PrefabActorFactory::GetPrefabActorContainer()
{
	return prefab_actor_container;
}

void PrefabActorFactory::ScanPrefabActorDirectory()
{
	std::string filepath = "Resource/Json/PrefabActor";
	for (auto & p : std::filesystem::directory_iterator{ filepath.c_str() })
	{
		if (p.path().extension() == ".json")
		{
			std::string slash = "/";
			std::string jsonfile = filepath + slash + p.path().filename().string();
			LoadPrefabActorFromJsonFile(jsonfile.c_str());
		}
	}
}

void PrefabActorFactory::LoadPrefabActorFromJsonFile(const char * filepath)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filepath, "r");
	std::cout << filepath << std::endl;

	if (err == 0)
	{
		char readBuffer[65536]; 
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); 
		rapidjson::Document document; 
		document.ParseStream(is); 
		if (document.HasMember("JsonFileType"))
		{
			std::string json_file_type = document["JsonFileType"].GetString();

			if (json_file_type.compare("Prefab") == 0)
			{
				if (document.HasMember("PrefabActor"))
				{
					std::string filename = filepath;
					std::size_t found = filename.find_last_of("/\\");
					filename = filename.substr((found + 1), filename.size());
					std::string prefab_type = "Unknown";
					if (document["PrefabActor"].HasMember("object_type_key"))
					{
						prefab_type = document["PrefabActor"]["object_type_key"].GetString();

					}

					PrefabActor * prefab_actor = new PrefabActor(filepath, filename, prefab_type);

				
					if (document["PrefabActor"].HasMember("prefab_name_key"))
					{
						prefab_actor->SetPrefabActorName(&prefab_actor_container, document["PrefabActor"]["prefab_name_key"].GetString());
					}
					else
					{
						prefab_actor->SetPrefabActorName(&prefab_actor_container, "Nameless");
					}


					if (document["PrefabActor"].HasMember("active"))
					{
						prefab_actor->SetActive(document["PrefabActor"]["active"].GetBool());
					}
					else
					{
						prefab_actor->SetActive(true);
					}


					if (document["PrefabActor"].HasMember("Component"))
					{
						const rapidjson::Value& compObj = document["PrefabActor"]["Component"];

						for (rapidjson::SizeType i = 0; i < compObj.Size(); i++)
						{
							std::string s = (compObj[i]["ComponentType"].GetString());
							Component * comp = InitComponent(s, compObj[i]);

							prefab_actor->AddComponent(comp);
						
						}
					}

					if (document["PrefabActor"].HasMember("prefab_actor_tagging"))
					{
						const rapidjson::Value& tagging = document["PrefabActor"]["prefab_actor_tagging"];

						for (rapidjson::SizeType i = 0; i < tagging.Size(); i++)
						{
							if (tagging[i].HasMember("prefab_actor_tagging_key") && tagging[i].HasMember("prefab_actor_tagging_value"))
							{
								prefab_actor->SetNewPrefabActorTagging(tagging[i]["prefab_actor_tagging_key"].GetString(), tagging[i]["prefab_actor_tagging_value"].GetString());
							}
						}
					}

					if (document["PrefabActor"].HasMember("child_prefab_actor_container"))
					{
						const rapidjson::Value& child_prefab_actor = document["PrefabActor"]["child_prefab_actor_container"];
						for (rapidjson::SizeType i = 0; i < child_prefab_actor.Size(); i++)
						{
							prefab_actor->AddChildPrefabActor(CreateChildPrefabActor(child_prefab_actor[i], prefab_actor));
						}
					}

					prefab_actor_container.emplace(prefab_actor_container_key, prefab_actor);

					++prefab_actor_container_key;
				}
			}
		}
		fclose(fp);
	}
	else
	{
		std::cout << "Fail to open " << filepath<< " file" << std::endl;
	}
}

PrefabActor * PrefabActorFactory::CreateChildPrefabActor(const rapidjson::Value& node, PrefabActor * parent_actor)
{
	std::string filepath = parent_actor->GetPath();
	std::string filename = parent_actor->GetFilename();

	std::string prefab_type = "Unknown";
	if (node.HasMember("object_type_key"))
	{
		prefab_type = node["object_type_key"].GetString();
	}

	PrefabActor * prefab_actor = new PrefabActor(filepath, filename, prefab_type, parent_actor);
	

	if (node.HasMember("prefab_name_key"))
	{
		prefab_actor->SetPrefabActorName(&prefab_actor_container, node["prefab_name_key"].GetString());
	}
	else
	{
		prefab_actor->SetPrefabActorName(&prefab_actor_container, "Nameless");
	}

	if (node.HasMember("active"))
	{
		prefab_actor->SetActive(node["active"].GetBool());
	}
	else
	{
		prefab_actor->SetActive(true);
	}

	if (node.HasMember("Component"))
	{
		const rapidjson::Value& compObj = node["Component"];

		for (rapidjson::SizeType i = 0; i < compObj.Size(); i++)
		{
			std::string s = (compObj[i]["ComponentType"].GetString());
			//std::cout << s << std::endl;
			Component * comp = InitComponent(s, compObj[i]);
			prefab_actor->AddComponent(comp);
			if (s.compare("TransformComponent") == 0)
			{
				auto parent_comp_map = parent_actor->GetComponentsPtr();
				auto comp_map = prefab_actor->GetComponentsPtr();
				if (parent_comp_map->find(ComponentType::TransformComponent) != parent_comp_map->end() && comp_map->find(ComponentType::TransformComponent) != comp_map->end())
				{
					TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp_map->find(ComponentType::TransformComponent)->second);
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map->find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(parent_trans);
				}
			}
		}
	}

	if (node.HasMember("prefab_actor_tagging"))
	{
		const rapidjson::Value& tagging = node["prefab_actor_tagging"];

		for (rapidjson::SizeType i = 0; i < tagging.Size(); i++)
		{
			if (tagging[i].HasMember("prefab_actor_tagging_key") && tagging[i].HasMember("prefab_actor_tagging_value"))
			{
				prefab_actor->SetNewPrefabActorTagging(tagging[i]["prefab_actor_tagging_key"].GetString(), tagging[i]["prefab_actor_tagging_value"].GetString());
			}
		}
	}

	if (node.HasMember("child_prefab_actor_container"))
	{
		const rapidjson::Value& child_prefab_actor = node["child_prefab_actor_container"];
		for (rapidjson::SizeType i = 0; i < child_prefab_actor.Size(); i++)
		{
			prefab_actor->AddChildPrefabActor(CreateChildPrefabActor(child_prefab_actor[i], prefab_actor));
		}
	}
	return prefab_actor;
}

void PrefabActorFactory::SaveAllPrefabActor()
{
	if (prefab_actor_container.size() > 0)
	{
		for (auto & elem : prefab_actor_container)
		{
			SavePrefabActor(elem.second);
		}
	}
}

void PrefabActorFactory::SavePrefabActor(PrefabActor * actor)
{
		// skip default prefab actor
		if (actor->GetPath().size() == 0)
			return;

		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

		std::stringstream file_name;
		file_name.str(std::string());
		//file_name << "Resource/Json/TestprefabActorSave/test" << actor->GetFilename();
		file_name << actor->GetPath();

		writer.SetMaxDecimalPlaces(6);

		writer.StartObject();
		writer.Key("JsonFileType");
		writer.String("Prefab");
		
		writer.Key("PrefabActor");

		writer.StartObject();

	
		writer.Key("object_type_key");
		writer.String(actor->GetObjectType().c_str());

		writer.Key("prefab_name_key");
		writer.String(actor->GetPrefabActorName().c_str());

		writer.Key("active");
		writer.Bool(actor->GetActive());

		if (actor->GetComponents().size() > 0)
		{
			writer.Key("Component");
			writer.StartArray();

			for (auto & elem2 : actor->GetComponents())
			{
				writer.StartObject();

				elem2.second->Unserialize(writer);

				writer.EndObject();
			}

			writer.EndArray();
		}

		if (actor->GetPrefabActorTagging().size() > 2)
		{
			writer.Key("prefab_actor_tagging");
			writer.StartArray();
			for (auto & elem2 : actor->GetPrefabActorTagging())
			{
				if (elem2.first.compare("object_type_key") != 0 && elem2.first.compare("prefab_name_key") != 0)
				{
					writer.StartObject();
					writer.Key("prefab_actor_tagging_key");
					writer.String(elem2.first.c_str());
					writer.Key("prefab_actor_tagging_value");
					writer.String(elem2.second.c_str());
					writer.EndObject();
				}
			}
			writer.EndArray();
		}

		if (actor->GetChildPrefabActors().size() > 0)
		{
			writer.Key("child_prefab_actor_container");
			writer.StartArray();
			for (auto & elem2 : actor->GetChildPrefabActors())
			{
				writer.StartObject();
				SaveChildPrefabActor(writer, elem2.second);
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
		{
			std::cout << file_name.str() << std::endl;
			throw std::runtime_error("Failed to save the prefab!");
		}
	
}

void PrefabActorFactory::SaveChildPrefabActor(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer, PrefabActor * child_prefab_actor)
{
	writer.Key("object_type_key");
	writer.String(child_prefab_actor->GetObjectType().c_str());

	writer.Key("prefab_name_key");
	writer.String(child_prefab_actor->GetPrefabActorName().c_str());

	writer.Key("active");
	writer.Bool(child_prefab_actor->GetActive());

	if (child_prefab_actor->GetComponents().size() > 0)
	{
		writer.Key("Component");
		writer.StartArray();

		for (auto & elem2 : child_prefab_actor->GetComponents())
		{
			writer.StartObject();

			elem2.second->Unserialize(writer);

			writer.EndObject();
		}

		writer.EndArray();
	}

	if (child_prefab_actor->GetPrefabActorTagging().size() > 2)
	{
		writer.Key("prefab_actor_tagging");
		writer.StartArray();
		for (auto & elem2 : child_prefab_actor->GetPrefabActorTagging())
		{
			if (elem2.first.compare("object_type_key") != 0 && elem2.first.compare("prefab_name_key") != 0)
			{
				writer.StartObject();
				writer.Key("prefab_actor_tagging_key");
				writer.String(elem2.first.c_str());
				writer.Key("prefab_actor_tagging_value");
				writer.String(elem2.second.c_str());
				writer.EndObject();
			}
		}
		writer.EndArray();
	}

	if (child_prefab_actor->GetChildPrefabActors().size() > 0)
	{
		writer.Key("child_prefab_actor_container");
		writer.StartArray();
		for (auto & elem2 : child_prefab_actor->GetChildPrefabActors())
		{
			writer.StartObject();
			SaveChildPrefabActor(writer, elem2.second);
			writer.EndObject();
		}
		writer.EndArray();
	}
}

std::unordered_map<std::string, std::string> & PrefabActorFactory::GetDefaultPrefabActorContainer()
{
	return default_prefab_actor_container;
}

std::unordered_map<unsigned, std::string> & PrefabActorFactory::GetObjectTypeContainer()
{
	return ObjectTypeList;
}

std::unordered_map<ComponentType, std::string> & PrefabActorFactory::GetComponentTypeContainer()
{
	return ComponentTypeList;
}

void PrefabActorFactory::ConstructDefaultPrefabActor()
{
	PrefabActor * default_prefab_actor = new PrefabActor("Custom Prefab", "Custom");
	default_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_prefab_actor);
	default_prefab_actor_container.emplace(default_prefab_actor->GetPrefabActorName(), "New Custom Prefab");
	++prefab_actor_container_key;

	PrefabActor * default_player_prefab_actor = new PrefabActor("Default Player Character", "Player");
	default_player_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_player_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_player_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_player_prefab_actor->AddComponent(InitDefaultComponent("CollisionComponent"));
	default_player_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	default_player_prefab_actor->AddComponent(InitDefaultComponent("MovementComponent"));
	default_player_prefab_actor->AddComponent(InitDefaultComponent("StatsComponent"));
	default_player_prefab_actor->AddComponent(InitDefaultComponent("CameraComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_player_prefab_actor);
	default_prefab_actor_container.emplace(default_player_prefab_actor->GetPrefabActorName(), "New Player Character");
	++prefab_actor_container_key;

	PrefabActor * default_land_enemy_prefab_actor = new PrefabActor("Default Enemy Character", "ENEMY");
	default_land_enemy_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_land_enemy_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_land_enemy_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_land_enemy_prefab_actor->AddComponent(InitDefaultComponent("CollisionComponent"));
	default_land_enemy_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	default_land_enemy_prefab_actor->AddComponent(InitDefaultComponent("MovementComponent"));
	default_land_enemy_prefab_actor->AddComponent(InitDefaultComponent("StatsComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_land_enemy_prefab_actor);
	default_prefab_actor_container.emplace(default_land_enemy_prefab_actor->GetPrefabActorName(), "New Enemy Character");
	++prefab_actor_container_key;

	PrefabActor * default_static_platform_prefab_actor = new PrefabActor("Default Static Platform", "Static_Platform");
	default_static_platform_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_static_platform_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_static_platform_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_static_platform_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_static_platform_prefab_actor);
	default_prefab_actor_container.emplace(default_static_platform_prefab_actor->GetPrefabActorName(), "New Static Platform");
	++prefab_actor_container_key;

	PrefabActor * default_moving_platform_prefab_actor = new PrefabActor("Default Moving Platform", "Moving_Platform");
	default_moving_platform_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_moving_platform_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_moving_platform_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_moving_platform_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_moving_platform_prefab_actor);
	default_prefab_actor_container.emplace(default_moving_platform_prefab_actor->GetPrefabActorName(), "New Moving Platform");
	++prefab_actor_container_key;

	PrefabActor * default_crumbling_platform_prefab_actor = new PrefabActor("Default Crumbling Platform", "Crumbling_Platform");
	default_crumbling_platform_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_crumbling_platform_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_crumbling_platform_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_crumbling_platform_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	default_crumbling_platform_prefab_actor->AddComponent(InitDefaultComponent("CollisionComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_crumbling_platform_prefab_actor);
	default_prefab_actor_container.emplace(default_crumbling_platform_prefab_actor->GetPrefabActorName(), "New Crumbling Platform");
	++prefab_actor_container_key;

	PrefabActor * default_oneway_platform_prefab_actor = new PrefabActor("Default Oneway Platform", "Oneway_Platform");
	default_oneway_platform_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_oneway_platform_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_oneway_platform_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_oneway_platform_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_oneway_platform_prefab_actor);
	default_prefab_actor_container.emplace(default_oneway_platform_prefab_actor->GetPrefabActorName(), "New Oneway Platform");
	++prefab_actor_container_key;

	PrefabActor * default_pit_traps_prefab_actor = new PrefabActor("Default Pit Traps", "Pit_Traps");
	default_pit_traps_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_pit_traps_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_pit_traps_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_pit_traps_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_pit_traps_prefab_actor);
	default_prefab_actor_container.emplace(default_pit_traps_prefab_actor->GetPrefabActorName(), "New Pit Traps");
	++prefab_actor_container_key;

	PrefabActor * default_timed_platform_prefab_actor = new PrefabActor("Default Timed Platform", "Timed_Platform");
	default_timed_platform_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_timed_platform_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_timed_platform_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_timed_platform_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_timed_platform_prefab_actor);
	default_prefab_actor_container.emplace(default_timed_platform_prefab_actor->GetPrefabActorName(), "New Timed Platform");
	++prefab_actor_container_key;

	PrefabActor * default_tornado_platform_prefab_actor = new PrefabActor("Default Tornado Platform", "Tornado_Platform");
	default_tornado_platform_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_tornado_platform_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_tornado_platform_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_tornado_platform_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_tornado_platform_prefab_actor);
	default_prefab_actor_container.emplace(default_tornado_platform_prefab_actor->GetPrefabActorName(), "New Tornado Platform");
	++prefab_actor_container_key;

	PrefabActor * background_prefab_actor = new PrefabActor("Default Background", "Background");
	background_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	background_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, background_prefab_actor);
	default_prefab_actor_container.emplace(background_prefab_actor->GetPrefabActorName(), "New Background");
	++prefab_actor_container_key;

	PrefabActor * invisible_wall_prefab_actor = new PrefabActor("Default Invisible Wall", "Invisible_Wall");
	invisible_wall_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	invisible_wall_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	invisible_wall_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, invisible_wall_prefab_actor);
	default_prefab_actor_container.emplace(invisible_wall_prefab_actor->GetPrefabActorName(), "New Invisible Wall");
	++prefab_actor_container_key;

	PrefabActor * default_switch_prefab_actor = new PrefabActor("Default Switch", "Switch");
	default_switch_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_switch_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_switch_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_switch_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	default_switch_prefab_actor->AddComponent(InitDefaultComponent("CollisionComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_switch_prefab_actor);
	default_prefab_actor_container.emplace(default_switch_prefab_actor->GetPrefabActorName(), "New Switch");
	++prefab_actor_container_key;

	PrefabActor * default_gate_prefab_actor = new PrefabActor("Default Gate", "Gate");
	default_gate_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	default_gate_prefab_actor->AddComponent(InitDefaultComponent("RigidBodyComponent"));
	default_gate_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	default_gate_prefab_actor->AddComponent(InitDefaultComponent("AABBComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, default_gate_prefab_actor);
	default_prefab_actor_container.emplace(default_gate_prefab_actor->GetPrefabActorName(), "New Gate");
	++prefab_actor_container_key;

	PrefabActor * new_default_button_prefab_actor = new PrefabActor("New Default Button", "New Button");
	new_default_button_prefab_actor->AddComponent(InitDefaultComponent("TransformComponent"));
	new_default_button_prefab_actor->AddComponent(InitDefaultComponent("ButtonComponent"));
	new_default_button_prefab_actor->AddComponent(InitDefaultComponent("SpriteComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, new_default_button_prefab_actor);
	default_prefab_actor_container.emplace(new_default_button_prefab_actor->GetPrefabActorName(), "New Default Button");
	++prefab_actor_container_key;

	PrefabActor * background_prefab_actor1 = new PrefabActor("Default BackgroundWithMusic", "BackgroundWithMusic");
	background_prefab_actor1->AddComponent(InitDefaultComponent("TransformComponent"));
	background_prefab_actor1->AddComponent(InitDefaultComponent("SpriteComponent"));
	background_prefab_actor1->AddComponent(InitDefaultComponent("AudioComponent"));
	prefab_actor_container.emplace(prefab_actor_container_key, background_prefab_actor1);
	default_prefab_actor_container.emplace(background_prefab_actor1->GetPrefabActorName(), "New BackgroundWithMusic");
	++prefab_actor_container_key;
	
}

unsigned PrefabActorFactory::NewPrefab(unsigned index)
{
	PrefabActor * new_prefab_actor = new PrefabActor(*(prefab_actor_container.find(index)->second));

	auto comp_map = new_prefab_actor->GetComponents();

	std::string name = default_prefab_actor_container.find(new_prefab_actor->GetPrefabActorName())->second;
	std::string tmp_path = "Resource/Json/PrefabActor/" + name + ".json";
	new_prefab_actor->SetPath(tmp_path);
	new_prefab_actor->SetPrefabActorName(&GetPrefabActorContainer(), name);
	unsigned count = 1;
	while (prefab_actor_container.find(count) != prefab_actor_container.end())
	{
		++count;
	}
	prefab_actor_container.emplace(count, new_prefab_actor);

	std::cout << "prefab no:" << count << std::endl;
	return count;
}

unsigned PrefabActorFactory::DuplicatePrefab(PrefabActor * duplicated_actor)
{
	PrefabActor * new_prefab_actor = new PrefabActor(*duplicated_actor);
	std::string tmp_path = "Resource/Json/PrefabActor/" + new_prefab_actor->GetPrefabActorName() + ".json";
	new_prefab_actor->SetPath(tmp_path);
	new_prefab_actor->SetPrefabActorName(&GetPrefabActorContainer(), new_prefab_actor->GetPrefabActorName());
	unsigned count = 1;
	while (prefab_actor_container.find(count) != prefab_actor_container.end())
	{
		++count;
	}
	prefab_actor_container.emplace(count, new_prefab_actor);
	return count;
}

void PrefabActorFactory::DeletePrefab(unsigned index)
{
	std::string path = prefab_actor_container.find(index)->second->GetPath();
	DeletePrefabFile(path);
	
	delete prefab_actor_container.find(index)->second;
	prefab_actor_container.find(index)->second = nullptr;
	prefab_actor_container.erase(index);
}

void PrefabActorFactory::DeletePrefabFile(std::string path)
{
	std::filesystem::remove_all(path);
}

void PrefabActorFactory::PrefabActorShiftSelection(PrefabActor * last_click)
{
	PrefabActor* backup = last_click;
	while (last_click->GetParentPrefabActorPointer() != nullptr)
	{
		backup = last_click;
		last_click = last_click->GetParentPrefabActorPointer();
	}
	PrefabActor* parent_prefab = last_click;
	last_click = backup;
	last_click->SetPrefabActorSelection(true);

	bool toggle = true;

	PrefabActor* top = nullptr;
	PrefabActor* bottom = nullptr;

	if (parent_prefab->GetChildPrefabActors().size() > 0)
	{
		for (auto elem : parent_prefab->GetChildPrefabActors())
		{
			if (elem.second == last_click)
			{
				toggle = false;
				continue;
			}

			if (toggle)
			{
				if (elem.second->GetPrefabActorSelection())
				{
					top = elem.second;
				}
			}
			else
			{
				if (elem.second->GetPrefabActorSelection())
				{
					bottom = elem.second;
					break;
				}
			}
		}

		if (top != nullptr)
		{
			bool begin_set_selection = false;
			for (auto elem : parent_prefab->GetChildPrefabActors())
			{
				if (elem.second == top)
				{
					begin_set_selection = true;
				}

				if (begin_set_selection)
				{
					elem.second->SetPrefabActorSelection(true);
				}

				if (elem.second == last_click)
				{
					break;
				}
			}
		}

		if (bottom != nullptr)
		{
			bool begin_set_selection = false;
			for (auto elem : parent_prefab->GetChildPrefabActors())
			{
				if (elem.second == last_click)
				{
					begin_set_selection = true;
				}

				if (begin_set_selection)
				{
					elem.second->SetPrefabActorSelection(true);
				}

				if (elem.second == bottom)
				{
					break;
				}
			}
		}
	}
}

// this only call in prefdab editor
void PrefabActorFactory::PrefabActorSelectionDelete(PrefabActor * prefab_actor)
{
	for (auto it = prefab_actor->GetChildPrefabActors().begin(); it != prefab_actor->GetChildPrefabActors().end(); )
	{
		PrefabActorSelectionDelete(it->second);

		if (it->second->GetPrefabActorSelection() == true)
		{
			//back up on parent layer
			PrefabActor* parent_prefab = it->second->GetParentPrefabActorPointer();
			// check got any child left, if yes need back up
			if (it->second->GetChildPrefabActors().size() > 0)
			{
				for (auto & elem : it->second->GetChildPrefabActors())
				{
					// relink all pointer information
					auto parent_map = parent_prefab->GetComponents();
					auto child_map = elem.second->GetComponents();

					// all prefab have transform component for sure, this just double check
					if (parent_map.find(ComponentType::TransformComponent) != parent_map.end() && child_map.find(ComponentType::TransformComponent) != child_map.end())
					{
						TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_map.find(ComponentType::TransformComponent)->second);
						TransformComponent* trans = dynamic_cast<TransformComponent*>(child_map.find(ComponentType::TransformComponent)->second);
						trans->SetParentTransformComponent(parent_trans);
					}

					// set new paren pointer
					elem.second->SetParentPrefabActorPointer(parent_prefab);
					parent_prefab->AddChildPrefabActor(elem.second);
				}
				// clear the child container, no delete require as move the pointer content
				it->second->GetChildPrefabActors().clear();

			}

			auto comp_map = it->second->GetComponents();
			if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
			{
				TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
				trans->SetParentTransformComponent(nullptr);
			}
			it->second->SetParentPrefabActorPointer(nullptr);
			delete it->second;
			it->second = nullptr;
			it = parent_prefab->GetChildPrefabActors().erase(it);
			
		}
		else
		{
			++it;
		}
	}
	//m_ev->PublishEvent(new SwitchLevelEvent{});
	//m_ev->PublishEvent(new LoadLevel{});

}

void PrefabActorFactory::SetDefaultPrefabAttribute(PrefabActor * prefab_actor)
{
	auto comp_map = prefab_actor->GetComponents();
	if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
	{
		TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
		trans->SetParentTransformComponent(nullptr);
		trans->SetPosition(Vector2D{0.0f,0.0f});
		trans->SetRotate(0.0f);
		trans->SetScale(Vector3D{ 1.0f,1.0f,1.0f });
	}

	if (comp_map.find(ComponentType::AABBComponent) != comp_map.end())
	{
		AABBComponent* aabb = dynamic_cast<AABBComponent*>(comp_map.find(ComponentType::AABBComponent)->second);
		aabb->SetAABBScale(Vector2D{ 1.0f,1.0f });
		aabb->SetOffset(Vector2D{ 0.0f,0.0f });
	}

	if (comp_map.find(ComponentType::RigidBodyComponent) != comp_map.end())
	{
		RigidBodyComponent* rigi = dynamic_cast<RigidBodyComponent*>(comp_map.find(ComponentType::RigidBodyComponent)->second);
		rigi->SetDirection(Vector2D{ 0.0f,0.0f });
		rigi->SetStartTime(0.0f);
		rigi->SetDuration(0.0f);
	}

	// spirte comp?


}

void PrefabActorFactory::AutoCreatePrefab()
{
	std::string filepath1 = "Resource/Json/PrefabGenerator/Background";
	std::string filepath2 = "Resource/Json/PrefabGenerator/BackgroundWithMusic";
	std::string filepath3 = "Resource/Json/PrefabGenerator/New Button";

}

void PrefabActorFactory::InitDigipenLogoAndTeamLogo()
{
	background_logo = new PrefabActor();
	background_logo->AddComponent(InitDefaultComponent("TransformComponent"));
	background_logo->AddComponent(InitDefaultComponent("SpriteComponent"));

	auto& background_logo_comp = background_logo->GetComponents();
	if (background_logo_comp.find(ComponentType::TransformComponent) != background_logo_comp.end())
	{
		TransformComponent* background_logo_trans = dynamic_cast<TransformComponent*>(background_logo_comp.find(ComponentType::TransformComponent)->second);
		background_logo_trans->SetTranslate(Vector3D(640.0f, 400.0f, 0.0f));
		background_logo_trans->SetScale(Vector3D(1280.0f, 800.0f, 0.0f));
	}

	if (background_logo_comp.find(ComponentType::SpriteComponent) != background_logo_comp.end())
	{
		SpriteComponent* background_logo_sprite = dynamic_cast<SpriteComponent*>(background_logo_comp.find(ComponentType::SpriteComponent)->second);
		unsigned key = GetSpriteKeyBySpriteName(m_SpriteSheetFactory->GetContainer(), "Black Background");
		auto tex = m_SpriteSheetFactory->GetContainer()->find(key)->second;

		background_logo_sprite->SetTexture(tex);
		background_logo_sprite->SetSprite("Black Background");


		background_logo_sprite->SetOrthCam(true);
		background_logo_sprite->SetLayer(0);
	}


	digipen_logo = new PrefabActor();
	digipen_logo->AddComponent(InitDefaultComponent("TransformComponent"));
	digipen_logo->AddComponent(InitDefaultComponent("SpriteComponent"));

	auto & digipen_logo_comp = digipen_logo->GetComponents();
	if (digipen_logo_comp.find(ComponentType::TransformComponent) != digipen_logo_comp.end())
	{
		TransformComponent* digipen_logo_trans = dynamic_cast<TransformComponent*>(digipen_logo_comp.find(ComponentType::TransformComponent)->second);
		digipen_logo_trans->SetTranslate(Vector3D(640.0f, 400.0f, 0.0f));
		digipen_logo_trans->SetScale(Vector3D(965.0f, 500.0f, 0.0f));
	}

	if (digipen_logo_comp.find(ComponentType::SpriteComponent) != digipen_logo_comp.end())
	{
		SpriteComponent* digipen_logo_sprite = dynamic_cast<SpriteComponent*>(digipen_logo_comp.find(ComponentType::SpriteComponent)->second);
		unsigned key = GetSpriteKeyBySpriteName(m_SpriteSheetFactory->GetContainer(), "Digipen Logo");
		auto tex = m_SpriteSheetFactory->GetContainer()->find(key)->second;

		digipen_logo_sprite->SetTexture(tex);
		digipen_logo_sprite->SetSprite("Digipen Logo");


		digipen_logo_sprite->SetOrthCam(true);
		digipen_logo_sprite->SetLayer(0);
	}

	team_logo = new PrefabActor();
	team_logo->AddComponent(InitDefaultComponent("TransformComponent"));
	team_logo->AddComponent(InitDefaultComponent("SpriteComponent"));

	auto & team_logo_comp = team_logo->GetComponents();
	if (team_logo_comp.find(ComponentType::TransformComponent) != team_logo_comp.end())
	{
		TransformComponent* team_logo_trans = dynamic_cast<TransformComponent*>(team_logo_comp.find(ComponentType::TransformComponent)->second);
		team_logo_trans->SetTranslate(Vector3D(640.0f, 400.0f, 0.0f));
		team_logo_trans->SetScale(Vector3D(800.0f, 800.0f, 0.0f));
	}

	if (team_logo_comp.find(ComponentType::SpriteComponent) != team_logo_comp.end())
	{
		SpriteComponent* team_logo_sprite = dynamic_cast<SpriteComponent*>(team_logo_comp.find(ComponentType::SpriteComponent)->second);
		unsigned key = GetSpriteKeyBySpriteName(m_SpriteSheetFactory->GetContainer(), "Team Logo");
		auto tex = m_SpriteSheetFactory->GetContainer()->find(key)->second;

		team_logo_sprite->SetTexture(tex);
		team_logo_sprite->SetSprite("Team Logo");


		team_logo_sprite->SetOrthCam(true);
		team_logo_sprite->SetLayer(0);
	}

	ConstructDefaultPrefabActor();
}