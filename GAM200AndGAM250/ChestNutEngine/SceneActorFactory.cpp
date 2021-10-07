/******************************************************************************/
/*!
\file   SceneActorFactory.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	the file contain the main scene container, loading and saving and the 
	function of edit in the editor, like select, new ,move, duplicate,
	rename,delete

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "SceneActorFactory.h"

SceneActorFactory::SceneActorFactory(PrefabActorFactory * prefabactor_fac, MainWindow * window, EventHandler * eventhandle, GraphicsSystem* graphic)
	:m_PrefabActorFactory{ prefabactor_fac }, m_win{ window }, m_ev{ eventhandle }, m_graphic{ graphic },
	scene_actor_container_key{ 1 }, current_scene_camera{nullptr}, 
global_coordinate_min_x{ 0 },global_coordinate_max_x{ 0  },
global_coordinate_min_y{ 0 },global_coordinate_max_y{ 0  },
check_level_size{ false }, first_actor_map_check{ false }
{
	
}

SceneActorFactory::~SceneActorFactory()
{
	for (auto & elem2 : loading_container)
	{
		delete elem2.second;
		elem2.second = nullptr;
	}
	loading_container.clear();

	for (auto & elem : scene_actor_container)
	{
		delete elem.second;
		elem.second = nullptr;
	}
	scene_actor_container.clear();

}

void SceneActorFactory::InitLoadingScreen()
{
	// background
	SceneActor * loading_background = new SceneActor();

	PrefabActor * loading_background_prefab = new PrefabActor();

	loading_background_prefab->AddComponent(m_PrefabActorFactory->InitDefaultComponent("TransformComponent"));
	loading_background_prefab->AddComponent(m_PrefabActorFactory->InitDefaultComponent("SpriteComponent"));
	loading_background_prefab->AddComponent(m_PrefabActorFactory->InitDefaultComponent("AudioComponent"));

	auto & loading_background_component = loading_background_prefab->GetComponents();
	if (loading_background_component.find(ComponentType::TransformComponent) != loading_background_component.end())
	{
		TransformComponent* loading_background_trans = dynamic_cast<TransformComponent*>(loading_background_component.find(ComponentType::TransformComponent)->second);
		loading_background_trans->SetTranslate(Vector3D(640.0f, 400.0f, 0.0f));
		loading_background_trans->SetScale(Vector3D(1280.0f,800.0f, 0.0f));
	}

	if (loading_background_component.find(ComponentType::SpriteComponent) != loading_background_component.end())
	{
		SpriteComponent* loading_background_sprite = dynamic_cast<SpriteComponent*>(loading_background_component.find(ComponentType::SpriteComponent)->second);
		unsigned key = GetSpriteKeyBySpriteName(m_PrefabActorFactory->m_SpriteSheetFactory->GetContainer(), "Black Background");
		auto tex = m_PrefabActorFactory->m_SpriteSheetFactory->GetContainer()->find(key)->second;

		loading_background_sprite->SetTexture(tex);
		loading_background_sprite->SetSprite("Black Background");


		loading_background_sprite->SetOrthCam(true);
		loading_background_sprite->SetLayer(0);
	}

	loading_background->SetPrefabActor(loading_background_prefab);

	loading_container.emplace(1, loading_background);

	// progress loading

	SceneActor * loading_progress = new SceneActor();

	PrefabActor * loading_progress_prefab = new PrefabActor();

	loading_progress_prefab->AddComponent(m_PrefabActorFactory->InitDefaultComponent("TransformComponent"));
	loading_progress_prefab->AddComponent(m_PrefabActorFactory->InitDefaultComponent("SpriteComponent"));

	auto & loading_progress_component = loading_progress_prefab->GetComponents();
	if (loading_progress_component.find(ComponentType::TransformComponent) != loading_progress_component.end())
	{
		TransformComponent* loading_progress_trans = dynamic_cast<TransformComponent*>(loading_progress_component.find(ComponentType::TransformComponent)->second);
		loading_progress_trans->SetTranslate(Vector3D(640.0f, 400.0f, 0.0f));
		loading_progress_trans->SetScale(Vector3D(877.0f, 242.0f, 0.0f));
	}

	if (loading_progress_component.find(ComponentType::SpriteComponent) != loading_progress_component.end())
	{
		SpriteComponent* loading_progress_sprite = dynamic_cast<SpriteComponent*>(loading_progress_component.find(ComponentType::SpriteComponent)->second);
		unsigned key = GetSpriteKeyBySpriteName(m_PrefabActorFactory->m_SpriteSheetFactory->GetContainer(), "Overthrone_Title");
		auto tex = m_PrefabActorFactory->m_SpriteSheetFactory->GetContainer()->find(key)->second;

		loading_progress_sprite->SetTexture(tex);
		loading_progress_sprite->SetSpriteName("Overthrone_Title");
		loading_progress_sprite->SetSprite("Overthrone_Title");
		loading_progress_sprite->SetOrthCam(true);
		loading_progress_sprite->SetLayer(1);
	}

	loading_progress->SetPrefabActor(loading_progress_prefab);

	loading_container.emplace(2, loading_progress);

	SceneActor * loading_progress2 = new SceneActor();

	PrefabActor * loading_progress_prefab2 = new PrefabActor();

	loading_progress_prefab2->AddComponent(m_PrefabActorFactory->InitDefaultComponent("TransformComponent"));
	loading_progress_prefab2->AddComponent(m_PrefabActorFactory->InitDefaultComponent("SpriteComponent"));

	auto & loading_progress_component2 = loading_progress_prefab2->GetComponents();
	if (loading_progress_component2.find(ComponentType::TransformComponent) != loading_progress_component2.end())
	{
		TransformComponent* loading_progress_trans2 = dynamic_cast<TransformComponent*>(loading_progress_component2.find(ComponentType::TransformComponent)->second);
		loading_progress_trans2->SetTranslate(Vector3D(1200.0f, 100.0f, 0.0f));
		loading_progress_trans2->SetScale(Vector3D(70.0f, 100.0f, 0.0f));
		progress_walk = loading_progress_trans2;
	}

	if (loading_progress_component2.find(ComponentType::SpriteComponent) != loading_progress_component2.end())
	{
		SpriteComponent* loading_progress_sprite2 = dynamic_cast<SpriteComponent*>(loading_progress_component2.find(ComponentType::SpriteComponent)->second);
		unsigned key = GetSpriteKeyBySpriteName(m_PrefabActorFactory->m_SpriteSheetFactory->GetContainer(), "Checkpoint");
		auto tex = m_PrefabActorFactory->m_SpriteSheetFactory->GetContainer()->find(key)->second;

		loading_progress_sprite2->SetTexture(tex);
		loading_progress_sprite2->SetSpriteName("RIGHT_Checkpoint_Lit");
		loading_progress_sprite2->SetSprite("RIGHT_Checkpoint_Lit");
		loading_progress_sprite2->SetOrthCam(true);
		loading_progress_sprite2->SetLayer(1);
		loading_progress_sprite2->SetFrameCount(1);
		progress_frame = loading_progress_sprite2;
	}

	loading_progress2->SetPrefabActor(loading_progress_prefab2);

	loading_container.emplace(3, loading_progress2);

	
}

void SceneActorFactory::InitGlobalCoordinate(float pos_x, float pos_y, float size_x, float size_y, float rot)
{
	float min_x = 0.0f;
	float max_x = 0.0f;
	float min_y = 0.0f;
	float max_y = 0.0f;

	UNREFERENCED_PARAMETER(rot);

	/*if (rot != 0)
	{
		Vector2D point1_without_rot = { pos_x - size_x * 0.5f , pos_y + size_y * 0.5f };
		Vector2D point2_without_rot = { pos_x + size_x * 0.5f , pos_y + size_y * 0.5f };
		Vector2D point3_without_rot = { pos_x - size_x * 0.5f , pos_y - size_y * 0.5f };
		Vector2D point4_without_rot = { pos_x + size_x * 0.5f , pos_y - size_y * 0.5f };

		std::vector<float> point_x;
		std::vector<float> point_y;

		point_x.push_back(cos(rot)*point1_without_rot.x - sin(rot)*point1_without_rot.y);
		point_x.push_back(cos(rot)*point2_without_rot.x - sin(rot)*point2_without_rot.y);
		point_x.push_back(cos(rot)*point3_without_rot.x - sin(rot)*point3_without_rot.y);
		point_x.push_back(cos(rot)*point4_without_rot.x - sin(rot)*point4_without_rot.y);

		point_y.push_back(sin(rot)*point1_without_rot.x + cos(rot)*point1_without_rot.y);
		point_y.push_back(sin(rot)*point2_without_rot.x + cos(rot)*point2_without_rot.y);
		point_y.push_back(sin(rot)*point3_without_rot.x + cos(rot)*point3_without_rot.y);
		point_y.push_back(sin(rot)*point4_without_rot.x + cos(rot)*point4_without_rot.y);

		min_x = *std::min_element(point_x.begin(), point_x.end());
		max_x = *std::max_element(point_x.begin(), point_x.end());
		min_y = *std::min_element(point_y.begin(), point_y.end());
		max_y = *std::max_element(point_y.begin(), point_y.end());
	}*/
	//else
	//{
		std::cout << "test " << pos_x << std::endl;
		min_x = pos_x - size_x * 0.5f;
		max_x = pos_x + size_x * 0.5f;
		min_y = pos_y - size_y * 0.5f;
		max_y = pos_y + size_y * 0.5f;

	//}
	global_coordinate_min_x = min_x;
	global_coordinate_max_x = max_x;
	global_coordinate_min_y = min_y;
	global_coordinate_max_y = max_y;

	std::cout << "min_x:" << min_x << " max_x:" << max_x << " min_y:" << min_y << " max_y:" << max_y << std::endl;
	std::cout << "min_x:" << global_coordinate_min_x << " max_x:" << global_coordinate_max_x << " min_y:" << global_coordinate_min_y << " max_y:" << global_coordinate_max_y << std::endl;
}

float SceneActorFactory::GetGlobalCoordinateMinX()const
{
	return global_coordinate_min_x;
}

float SceneActorFactory::GetGlobalCoordinateMaxX()const
{
	return global_coordinate_max_x;
}

float SceneActorFactory::GetGlobalCoordinateMinY()const
{
	return global_coordinate_min_y;
}

float SceneActorFactory::GetGlobalCoordinateMaxY()const
{
	return global_coordinate_max_y;
}

void SceneActorFactory::ReadtLevelSize(float min_x, float max_x, float min_y, float max_y)
{
	global_coordinate_min_x = min_x;
	global_coordinate_max_x = max_x;
	global_coordinate_min_y = min_y;
	global_coordinate_max_y = max_y;
}
void SceneActorFactory::CalculateLevelSize(float pos_x, float pos_y, float size_x, float size_y, float rot)
{
	float min_x = 0.0f;
	float max_x = 0.0f;
	float min_y = 0.0f;
	float max_y = 0.0f;

	UNREFERENCED_PARAMETER(rot);

	/*if (rot != 0)
	{
		Vector2D point1_without_rot = { pos_x - size_x * 0.5f , pos_y + size_y * 0.5f };
		Vector2D point2_without_rot = { pos_x + size_x * 0.5f , pos_y + size_y * 0.5f };
		Vector2D point3_without_rot = { pos_x - size_x * 0.5f , pos_y - size_y * 0.5f };
		Vector2D point4_without_rot = { pos_x + size_x * 0.5f , pos_y - size_y * 0.5f };

		std::vector<float> point_x;
		std::vector<float> point_y;

		point_x.push_back(cos(rot)*point1_without_rot.x - sin(rot)*point1_without_rot.y);
		point_x.push_back(cos(rot)*point2_without_rot.x - sin(rot)*point2_without_rot.y);
		point_x.push_back(cos(rot)*point3_without_rot.x - sin(rot)*point3_without_rot.y);
		point_x.push_back(cos(rot)*point4_without_rot.x - sin(rot)*point4_without_rot.y);

		point_y.push_back(sin(rot)*point1_without_rot.x + cos(rot)*point1_without_rot.y);
		point_y.push_back(sin(rot)*point2_without_rot.x + cos(rot)*point2_without_rot.y);
		point_y.push_back(sin(rot)*point3_without_rot.x + cos(rot)*point3_without_rot.y);
		point_y.push_back(sin(rot)*point4_without_rot.x + cos(rot)*point4_without_rot.y);

		min_x = *std::min_element(point_x.begin(), point_x.end()) ;
		max_x = *std::max_element(point_x.begin(), point_x.end()) ;
		min_y = *std::min_element(point_y.begin(), point_y.end()) ;
		max_y = *std::max_element(point_y.begin(), point_y.end()) ;
	}*/
	//else
	//{
		min_x = pos_x - size_x * 0.5f ;
		max_x = pos_x + size_x * 0.5f ;
		min_y = pos_y - size_y * 0.5f ;
		max_y = pos_y + size_y * 0.5f ;

	//}

	if (global_coordinate_min_x > min_x)
		global_coordinate_min_x = min_x;

	if (global_coordinate_max_x < max_x)
		global_coordinate_max_x = max_x;

	if (global_coordinate_min_y > min_y)
		global_coordinate_min_y = min_y;

	if (global_coordinate_max_y < max_y)
		global_coordinate_max_y = max_y;

	std::cout << "min_x:" << min_x << " max_x:" << max_x << " min_y:" << min_y << " max_y:" << max_y << std::endl;
	std::cout << "min_x:" << global_coordinate_min_x << " max_x:" << global_coordinate_max_x << " min_y:" << global_coordinate_min_y << " max_y:" << global_coordinate_max_y << std::endl;
}


void SceneActorFactory::LoadSceneActorFromJsonFile(const char * filepath)
{	
	FILE* fp;
	errno_t err;
	const float dt = 1.f / 60;
	float change_frame_timer = 0.0f;
	err = fopen_s(&fp, filepath, "r");
	if (err == 0)
	{
		//CameraComponent* setclamplimit_cam = nullptr;
		first_actor_map_check = true;
		check_level_size = true;
		std::cout << "Loading content of " << filepath << " file" << std::endl;

		std::string hard_code_map_size = filepath;
		const size_t last_slash_idx = hard_code_map_size.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
		{
			hard_code_map_size.erase(0, last_slash_idx + 1);
		}

		char readBuffer[65536]; 
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); 
		rapidjson::Document document; 
		document.ParseStream(is); 
		if (document.HasMember("JsonFileType"))
		{
			std::string json_file_type = document["JsonFileType"].GetString();
			if (json_file_type.compare("Level") == 0)
			{
				/*if(document.HasMember("LevelCoordinate") )
				{
					if(document["LevelCoordinate"].HasMember("global_coordinate_min_x") && document["LevelCoordinate"].HasMember("global_coordinate_max_x")
						&& document["LevelCoordinate"].HasMember("global_coordinate_min_y") && document["LevelCoordinate"].HasMember("global_coordinate_max_y"))
					{
						ReadLevelSize(document["LevelCoordinate"]["global_coordinate_min_x"].GetFloat(), document["LevelCoordinate"]["global_coordinate_max_x"].GetFloat(),
									document["LevelCoordinate"]["global_coordinate_min_y"].GetFloat(), document["LevelCoordinate"]["global_coordinate_max_y"].GetFloat());
						check_level_size = true;
					}
				}*/
				if (document.HasMember("SceneActor"))
				{
					const rapidjson::Value& scene_actor_json = document["SceneActor"];

					// init progress bar which is surter
					//float progress_x = 100.0f;
					//progress_walk->SetTranslate(Vector3D(100.0f, 100.0f, 0.0f));
					unsigned current_frame = 1;
					progress_frame->SetFrameCount(current_frame);
					//float move_per_load = (1280.0f - 150.0f) / scene_actor_json.Size();
					
			
					for (rapidjson::SizeType a = 0; a < scene_actor_json.Size(); a++)
					{
						unsigned prefab_actor_container_key = 0;
						if (scene_actor_json[a].HasMember("scene_actor_prefab_type_key"))
						{
							std::string not_prefab = scene_actor_json[a]["scene_actor_prefab_type_key"].GetString();

							if (not_prefab.compare("NotPrefab") == 0)
							{
								SceneActor * scene_actor = new SceneActor();

								if (scene_actor_json[a].HasMember("scene_actor_name_key"))
								{
									scene_actor->SetSceneActorName(&scene_actor_container, scene_actor_json[a]["scene_actor_name_key"].GetString());
								}
								else
								{
									scene_actor->SetSceneActorName(&scene_actor_container, scene_actor->GetSceneActorName());
								}

								if (scene_actor_json[a].HasMember("active"))
								{
									scene_actor->SetOnlyActive(scene_actor_json[a]["active"].GetBool());
								}
								else
								{
									scene_actor->SetOnlyActive(true);
								}

								PrefabActor * prefab_actor = new PrefabActor();
								prefab_actor->SetPath("!");

								if (scene_actor_json[a].HasMember("Component"))
								{
									const rapidjson::Value& compObj = scene_actor_json[a]["Component"];

									for (rapidjson::SizeType i = 0; i < compObj.Size(); i++)
									{
										std::string s = (compObj[i]["ComponentType"].GetString());
										Component * comp = m_PrefabActorFactory->InitComponent(s, compObj[i]);

										prefab_actor->AddComponent(comp);

									}
								}
								scene_actor->SetPrefabActor(prefab_actor);

								if (scene_actor_json[a].HasMember("scene_actor_tagging"))
								{
									const rapidjson::Value& tagging = scene_actor_json[a]["scene_actor_tagging"];
									for (rapidjson::SizeType i = 0; i < tagging.Size(); i++)
									{
										if (tagging[i].HasMember("scene_actor_tagging_key") && tagging[i].HasMember("scene_actor_tagging_value"))
										{
											scene_actor->SetNewSceneActorTagging(tagging[i]["scene_actor_tagging_key"].GetString(), tagging[i]["scene_actor_tagging_value"].GetString());
										}
									}
								}
								auto & comps = scene_actor->GetPrefabActor()->GetComponents();

							
								if (check_level_size &&  comps.find(ComponentType::TransformComponent) != comps.end() && comps.find(ComponentType::SpriteComponent) != comps.end())
								{
									SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(comps.find(ComponentType::SpriteComponent)->second);
									if (!sprite->IsOrthCam())
									{
										std::cout << scene_actor->GetSceneActorName() << "checked" << std::endl;
										TransformComponent* trans = dynamic_cast<TransformComponent*>(comps.find(ComponentType::TransformComponent)->second);
										if (first_actor_map_check)
										{
											InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
											first_actor_map_check = false;
										}
										else
										{
											CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
										}
									}

								}
								if (scene_actor_json[a].HasMember("child_scene_actor_container"))
								{
									const rapidjson::Value& child_scene_actor = scene_actor_json[a]["child_scene_actor_container"];
									for (rapidjson::SizeType i = 0; i < child_scene_actor.Size(); i++)
									{
										SceneActor * child_sceneactor = CreateChildSceneActor(child_scene_actor[i], scene_actor);

										if (child_sceneactor != nullptr)
										{
											scene_actor->AddChildSceneActor(child_sceneactor);
										}
									}
								}
								scene_actor_container.emplace(scene_actor_container_key, scene_actor);
								++scene_actor_container_key;
							}
							else
							{
								if (scene_actor_json[a].HasMember("path_key"))
								{
									std::string path = scene_actor_json[a]["path_key"].GetString();
							
									if (!CheckPrefabActorFileLoaded(&m_PrefabActorFactory->GetPrefabActorContainer(), path))
									{
										m_PrefabActorFactory->LoadPrefabActorFromJsonFile(path.c_str());
									}
									prefab_actor_container_key = GetPrefabKeyByPath(&m_PrefabActorFactory->GetPrefabActorContainer(), path);

									if (prefab_actor_container_key == 0)
									{
										std::cout << std::endl;
										std::cout << "fatal error" << std::endl;
										if (scene_actor_json[a].HasMember("object_type_key"))
										{
											std::string object_type = scene_actor_json[a]["object_type_key"].GetString();
											if (object_type.compare("Default") == 0 || object_type.compare("Custom") == 0)
											{
												std::cout << "Fatal Error , custom type and can't found the prefab" << std::endl;
											}
											else
											{
												prefab_actor_container_key = GetPrefabKeyByName(&m_PrefabActorFactory->GetPrefabActorContainer(), object_type);
												std::cout << "prefab can't be found, it will use default " << object_type << " instead." << std::endl;
											}
										}
										else
										{
											//std::cout << "cannot find \"" << path << "\" prefab, and no object type stated, it will use default prefab instead." << std::endl;
										}
									}
								}
								else
								{
									if (scene_actor_json[a].HasMember("object_type_key"))
									{
										std::string object_type = scene_actor_json[a]["object_type_key"].GetString();
										prefab_actor_container_key = GetPrefabKeyByName(&m_PrefabActorFactory->GetPrefabActorContainer(), object_type);
										std::cout << "prefab can't be found, it will use default " << object_type << " instead." << std::endl;
									}
									else
									{
										std::cout << "not key path and object type given, it will use a default prefab instead." << std::endl;
									}
								}

								SceneActor * scene_actor = new SceneActor(m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_container_key)->second);

								if (scene_actor_json[a].HasMember("scene_actor_name_key"))
								{
									scene_actor->SetSceneActorName(&scene_actor_container, scene_actor_json[a]["scene_actor_name_key"].GetString());
								}
								else
								{
									scene_actor->SetSceneActorName(&scene_actor_container, scene_actor->GetSceneActorName());
								}

								if (scene_actor_json[a].HasMember("active"))
								{
									scene_actor->SetOnlyActive(scene_actor_json[a]["active"].GetBool());
								}
								else
								{
									scene_actor->SetOnlyActive(true);
								}

								auto comps = scene_actor->GetPrefabActor()->GetComponentsPtr();


								// some stats edit
								// old
								if (scene_actor_json[a].HasMember("m_pos.x") && scene_actor_json[a].HasMember("m_pos.y"))
								{
									if (comps->find(ComponentType::TransformComponent) != comps->end())
									{
										TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
										trans->SetPosition(Vector2D(scene_actor_json[a]["m_pos.x"].GetFloat(), scene_actor_json[a]["m_pos.y"].GetFloat()));
									}
								}
								// new
								if (scene_actor_json[a].HasMember("m_Pos.x") && scene_actor_json[a].HasMember("m_Pos.y"))
								{
									if (comps->find(ComponentType::TransformComponent) != comps->end())
									{
										TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
										trans->SetPosition(Vector2D(scene_actor_json[a]["m_Pos.x"].GetFloat(), scene_actor_json[a]["m_Pos.y"].GetFloat()));
									}
								}

								if (scene_actor_json[a].HasMember("m_Rotate"))
								{
									if (comps->find(ComponentType::TransformComponent) != comps->end())
									{
										TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
										trans->SetRotate(scene_actor_json[a]["m_Rotate"].GetFloat());
									}
								}
								if (scene_actor_json[a].HasMember("m_Scale.x") && scene_actor_json[a].HasMember("m_Scale.y"))
								{
									if (comps->find(ComponentType::TransformComponent) != comps->end())
									{
										TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
										trans->SetScale(Vector3D(scene_actor_json[a]["m_Scale.x"].GetFloat(), scene_actor_json[a]["m_Scale.y"].GetFloat(), 0));

									}
								}
								if (scene_actor_json[a].HasMember("m_aabbScale.x") && scene_actor_json[a].HasMember("m_aabbScale.y"))
								{
									if (comps->find(ComponentType::AABBComponent) != comps->end())
									{
										AABBComponent* aabb = dynamic_cast<AABBComponent*>(comps->find(ComponentType::AABBComponent)->second);
										aabb->SetAABBScale(Vector2D(scene_actor_json[a]["m_aabbScale.x"].GetFloat(), scene_actor_json[a]["m_aabbScale.y"].GetFloat()));
									}
								}
								if (scene_actor_json[a].HasMember("m_offset.x") && scene_actor_json[a].HasMember("m_offset.y"))
								{
									if (comps->find(ComponentType::AABBComponent) != comps->end())
									{
										AABBComponent* aabb = dynamic_cast<AABBComponent*>(comps->find(ComponentType::AABBComponent)->second);
										aabb->SetOffset(Vector2D(scene_actor_json[a]["m_offset.x"].GetFloat(), scene_actor_json[a]["m_offset.y"].GetFloat()));
									}
								}
								if (scene_actor_json[a].HasMember("m_direction.x") && scene_actor_json[a].HasMember("m_direction.y"))
								{
									if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
									{
										RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
										rb->SetDirection(Vector2D(scene_actor_json[a]["m_direction.x"].GetFloat(), scene_actor_json[a]["m_direction.y"].GetFloat()));
									}
								}
								if (scene_actor_json[a].HasMember("m_startTime"))
								{
									if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
									{
										RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
										rb->SetStartTime(float(scene_actor_json[a]["m_startTime"].GetFloat()));
									}
								}
								if (scene_actor_json[a].HasMember("Button ID"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_buttonID = scene_actor_json[a]["Button ID"].GetInt();
									}
								}

								if (scene_actor_json[a].HasMember("Icon"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_icon = static_cast<Icon>(scene_actor_json[a]["Icon"].GetInt());
									}
								}

								if (scene_actor_json[a].HasMember("Section ID"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_sectionID = scene_actor_json[a]["Section ID"].GetInt();
									}
								}

								if (scene_actor_json[a].HasMember("File path"))
								{
									if (comps->find(ComponentType::CheckpointComponent) != comps->end())
									{
										CheckpointComponent * checkpoint = dynamic_cast<CheckpointComponent*>(comps->find(ComponentType::CheckpointComponent)->second);
										checkpoint->m_path = scene_actor_json[a]["File path"].GetString();
									}
								}

								if (scene_actor_json[a].HasMember("Active Texture"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->SetActiveTexName(scene_actor_json[a]["Active Texture"].GetString());
									}
								}

								if (scene_actor_json[a].HasMember("Inactive Texture"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->SetInactiveTexName(scene_actor_json[a]["Inactive Texture"].GetString());
									}
								}

								if (scene_actor_json[a].HasMember("Trigger Type"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_type = static_cast<TriggerType>(scene_actor_json[a]["Trigger Type"].GetInt());
									}
								}

								if (scene_actor_json[a].HasMember("Game State"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->SetGameStatePath(scene_actor_json[a]["Game State"].GetString());
									}
								}


								if (scene_actor_json[a].HasMember("Menu ID"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_menuID = scene_actor_json[a]["Menu ID"].GetInt();
									}
								}

								if (scene_actor_json[a].HasMember("Enter"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_enter = scene_actor_json[a]["Enter"].GetString();
									}
								}

								if (scene_actor_json[a].HasMember("Navigate"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_navigate = scene_actor_json[a]["Navigate"].GetString();
									}
								}

								if (scene_actor_json[a].HasMember("isHovered"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_hovered = scene_actor_json[a]["isHovered"].GetBool();
									}
								}

								if (scene_actor_json[a].HasMember("isMenu"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_isMenu = scene_actor_json[a]["isMenu"].GetBool();
									}
								}


								if (scene_actor_json[a].HasMember("Next Menu"))
								{
									if (comps->find(ComponentType::ButtonComponent) != comps->end())
									{
										ButtonComponent * button = dynamic_cast<ButtonComponent*>(comps->find(ComponentType::ButtonComponent)->second);
										button->m_next = scene_actor_json[a]["Next Menu"].GetInt();
									}
								}

								if (scene_actor_json[a].HasMember("m_duration"))
								{
									if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
									{
										RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
										rb->SetDuration(float(scene_actor_json[a]["m_duration"].GetFloat()));
									}
								}

								if (scene_actor_json[a].HasMember("m_modA"))
								{
									if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
									{
										RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
										rb->SetModA(float(scene_actor_json[a]["m_modA"].GetFloat()));
									}
								}

								if (scene_actor_json[a].HasMember("m_modB"))
								{
									if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
									{
										RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
										rb->SetModB(float(scene_actor_json[a]["m_modB"].GetFloat()));
									}
								}

								if (scene_actor_json[a].HasMember("m_modC"))
								{
									if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
									{
										RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
										rb->SetModC(float(scene_actor_json[a]["m_modC"].GetFloat()));
									}
								}

								bool non_orth = false;
								bool non_invisible = true;
								if (scene_actor_json[a].HasMember("Orthogonal_Camera"))
								{
									if (comps->find(ComponentType::SpriteComponent) != comps->end())
									{
										SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(comps->find(ComponentType::SpriteComponent)->second);
										sprite->SetOrthCam(scene_actor_json[a]["Orthogonal_Camera"].GetBool());
										non_orth = !scene_actor_json[a]["Orthogonal_Camera"].GetBool();
										std::string tex_name = sprite->GetSpriteName();
										if (tex_name.compare("Existence") == 0)
										{
											non_invisible = false;
										}
									}
								}

								if (scene_actor_json[a].HasMember("Layer"))
								{
									if (comps->find(ComponentType::SpriteComponent) != comps->end())
									{
										SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(comps->find(ComponentType::SpriteComponent)->second);
										sprite->SetLayer(scene_actor_json[a]["Layer"].GetUint());
									}
								}

								if (scene_actor_json[a].HasMember("DialogueID"))
								{
									if (comps->find(ComponentType::DialogueComponent) != comps->end())
									{
										DialogueComponent* dialogue = dynamic_cast<DialogueComponent*>(comps->find(ComponentType::DialogueComponent)->second);
										dialogue->m_id = scene_actor_json[a]["DialogueID"].GetUint();
									}
								}


								if (scene_actor_json[a].HasMember("Dialogues"))
								{
									if (comps->find(ComponentType::DialogueComponent) != comps->end())
									{
										DialogueComponent* dialogue = dynamic_cast<DialogueComponent*>(comps->find(ComponentType::DialogueComponent)->second);
										const rapidjson::Value& tagging = scene_actor_json[a]["Dialogues"];
										for (rapidjson::SizeType i = 0; i < tagging.Size(); i++)
										{
											if (tagging[i].HasMember("Dialogue Text"))
												dialogue->m_dialogues.push_back(tagging[i]["Dialogue Text"].GetString());
										}
									}
								}

								if (scene_actor_json[a].HasMember("cam_pos_x") && scene_actor_json[a].HasMember("cam_pos_y") && scene_actor_json[a].HasMember("cam_pos_z")
								&& scene_actor_json[a].HasMember("cam_front_x") && scene_actor_json[a].HasMember("cam_front_y") && scene_actor_json[a].HasMember("cam_front_z")
								&& scene_actor_json[a].HasMember("cam_up_x") && scene_actor_json[a].HasMember("cam_up_y") && scene_actor_json[a].HasMember("cam_up_z")
								&& scene_actor_json[a].HasMember("cam_offset_pos_x") && scene_actor_json[a].HasMember("cam_offset_pos_y"))
								{
									if (comps->find(ComponentType::CameraComponent) != comps->end())
									{
										CameraComponent* cam = dynamic_cast<CameraComponent*>(comps->find(ComponentType::CameraComponent)->second);
										cam->GetCamPos()->x = scene_actor_json[a]["cam_pos_x"].GetFloat();
										cam->GetCamPos()->y = scene_actor_json[a]["cam_pos_y"].GetFloat();
										cam->GetCamPos()->z = scene_actor_json[a]["cam_pos_z"].GetFloat();

										cam->GetCamFront()->x = scene_actor_json[a]["cam_front_x"].GetFloat();
										cam->GetCamFront()->y = scene_actor_json[a]["cam_front_y"].GetFloat();
										cam->GetCamFront()->z = scene_actor_json[a]["cam_front_z"].GetFloat();

										cam->GetCamUp()->x = scene_actor_json[a]["cam_up_x"].GetFloat();
										cam->GetCamUp()->y = scene_actor_json[a]["cam_up_y"].GetFloat();
										cam->GetCamUp()->z = scene_actor_json[a]["cam_up_z"].GetFloat();

										*cam->GetPosOffsetX() = scene_actor_json[a]["cam_offset_pos_x"].GetFloat();
										*cam->GetPosOffsetY() = scene_actor_json[a]["cam_offset_pos_y"].GetFloat();
										cam->GetCamera()->LatchToObject(scene_actor);

										if (scene_actor->GetSceneActorName().compare("Surtur") == 0)
										{
											//cam->GetCamera()->SetClampLimit(global_coordinate_min_x, global_coordinate_max_x, global_coordinate_min_y, global_coordinate_max_y);
											current_scene_camera = scene_actor;
										}
									}
								}


								if (scene_actor_json[a].HasMember("scene_actor_tagging"))
								{
									const rapidjson::Value& tagging = scene_actor_json[a]["scene_actor_tagging"];
									for (rapidjson::SizeType i = 0; i < tagging.Size(); i++)
									{
										if (tagging[i].HasMember("scene_actor_tagging_key") && tagging[i].HasMember("scene_actor_tagging_value"))
										{
											scene_actor->SetNewSceneActorTagging(tagging[i]["scene_actor_tagging_key"].GetString(), tagging[i]["scene_actor_tagging_value"].GetString());
										}
									}
								}

								if (check_level_size && non_orth && non_invisible && comps->find(ComponentType::TransformComponent) != comps->end() 
									&& scene_actor->GetPrefabActor()->GetPrefabActorName().compare("MetricGrid") != 0 && scene_actor->GetPrefabActor()->GetPrefabActorName().compare("FadeToBlack") !=0   )
								{
									TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
									std::cout << std::endl;
									std::cout << scene_actor->GetSceneActorName() << " map size checked" << std::endl;
									if (first_actor_map_check)
									{
										std::cout << "first!!!" << std::endl;
										InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
										first_actor_map_check = false;
									}
									else
									{
										CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
									}
									std::cout << std::endl;


								}

								if (scene_actor_json[a].HasMember("child_scene_actor_container"))
								{
									const rapidjson::Value& child_scene_actor = scene_actor_json[a]["child_scene_actor_container"];
									for (rapidjson::SizeType i = 0; i < child_scene_actor.Size(); i++)
									{
										SceneActor * child_sceneactor = CreateChildSceneActor(child_scene_actor[i], scene_actor);

										if (child_sceneactor != nullptr)
										{
											scene_actor->AddChildSceneActor(child_sceneactor);
										}
									}
								}

								scene_actor_container.emplace(scene_actor_container_key, scene_actor);
								++scene_actor_container_key;
							}
						}
						//progress_x += move_per_load;
						//progress_walk->SetTranslate(Vector3D(progress_x, 100.0f, 0));
						change_frame_timer += dt;
						if (change_frame_timer > 0.48f)
						{
							change_frame_timer = 0.0f;
							++current_frame;
						}
						if (current_frame > 5)
						{
							current_frame = 1;
						}
						progress_frame->SetFrameCount(current_frame);
						
						m_graphic->startDrawingScene();
						m_graphic->drawLoadScene(GetLoadingContainer());
						m_graphic->stopDrawingScene(*m_win->GetHDC());
						
					}
				}

			}
		}
		
		fclose(fp);

		if (scene_actor_container.find(GetSceneActorKeyByName(&scene_actor_container, "Surtur")) != scene_actor_container.end())
		{
			auto & comps = scene_actor_container.find(GetSceneActorKeyByName(&scene_actor_container, "Surtur"))->second->GetPrefabActor()->GetComponents();

			current_scene_camera = scene_actor_container.find(GetSceneActorKeyByName(&scene_actor_container, "Surtur"))->second;

			if (comps.find(ComponentType::CameraComponent) != comps.end())
			{
				CameraComponent * cam = dynamic_cast<CameraComponent*>(comps.find(ComponentType::CameraComponent)->second);
				if (hard_code_map_size.compare("Level001.json") == 0)
				{
					m_graphic->load_timer = 2.0f;
				}
				else if (hard_code_map_size.compare("Level002.json") == 0)
				{
					global_coordinate_min_x = 1.5f;
					global_coordinate_min_y = -90.0f;
					m_graphic->load_timer = 2.0f;
				}
				else if (hard_code_map_size.compare("Boss.json") == 0)
				{
					global_coordinate_min_x = -38.0f;
					global_coordinate_max_x = 38.0f;
					m_graphic->load_timer = 2.0f;
				}
				else
				{
					m_graphic->load_timer = 2.0f;
				}
				m_graphic->hidden_timer = 0.0f;
				cam->GetCamera()->SetClampLimit(global_coordinate_min_x, global_coordinate_max_x, global_coordinate_min_y, global_coordinate_max_y);
				//cam->GetCamera()->SetObjectAnchorPoint(7,5.0f,5.0f);

			}
		}
		else
		{
			m_graphic->load_timer = 2.0f;
		}
		
	}
	else
	{
		std::cout << "Fail to open " << filepath << " file" << std::endl;
	}


	


	m_ev->PublishEvent(new SwitchLevelEvent{});
	m_ev->PublishEvent(new LoadLevel{});
	m_ev->PublishEvent(new PlayLevel{});

	//m_ev->PublishEvent(new ChangeAudio{});
}

SceneActor * SceneActorFactory::CreateChildSceneActor(const rapidjson::Value& node, SceneActor * parent_actor)
{
	unsigned prefab_actor_container_key = 0;

	SceneActor * scene_actor = nullptr;

	if (node.HasMember("scene_actor_prefab_type_key"))
	{
		std::string not_prefab = node["scene_actor_prefab_type_key"].GetString();
		if (not_prefab.compare("NotPrefab") == 0)
		{
			scene_actor = new SceneActor();

			if (node.HasMember("scene_actor_name_key"))
			{
				scene_actor->SetSceneActorName(&scene_actor_container, node["scene_actor_name_key"].GetString());
			}
			else
			{
				scene_actor->SetSceneActorName(&scene_actor_container, scene_actor->GetSceneActorName());
			}

			if (node.HasMember("active"))
			{
				scene_actor->SetOnlyActive(node["active"].GetBool());
			}
			else
			{
				scene_actor->SetOnlyActive(true);
			}

			PrefabActor * prefab_actor = new PrefabActor();
			prefab_actor->SetPath("!");
			if (node.HasMember("Component"))
			{
				const rapidjson::Value& compObj = node["Component"];

				for (rapidjson::SizeType i = 0; i < compObj.Size(); i++)
				{
					std::string s = (compObj[i]["ComponentType"].GetString());
					Component * comp = m_PrefabActorFactory->InitComponent(s, compObj[i]);

					prefab_actor->AddComponent(comp);

				}
			}
			scene_actor->SetPrefabActor(prefab_actor);

			if (node.HasMember("scene_actor_tagging"))
			{
				const rapidjson::Value& tagging = node["scene_actor_tagging"];
				for (rapidjson::SizeType i = 0; i < tagging.Size(); i++)
				{
					if (tagging[i].HasMember("scene_actor_tagging_key") && tagging[i].HasMember("scene_actor_tagging_value"))
					{
						scene_actor->SetNewSceneActorTagging(tagging[i]["scene_actor_tagging_key"].GetString(), tagging[i]["scene_actor_tagging_value"].GetString());
					}
				}
			}
			auto & comps = scene_actor->GetPrefabActor()->GetComponents();

			if (check_level_size &&  comps.find(ComponentType::TransformComponent) != comps.end() && comps.find(ComponentType::SpriteComponent) != comps.end())
			{
				SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(comps.find(ComponentType::SpriteComponent)->second);
				if (!sprite->IsOrthCam())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comps.find(ComponentType::TransformComponent)->second);
					std::cout << std::endl;
					//std::cout << scene_actor->GetSceneActorName() << " map size checked" << std::endl;
					if (first_actor_map_check)
					{
						std::cout << "first!!!" << std::endl;
						InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
						first_actor_map_check = false;
					}
					else
					{
						CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
					}
					std::cout << std::endl;
				}

			}

			if (node.HasMember("child_scene_actor_container"))
			{
				const rapidjson::Value& child_scene_actor = node["child_scene_actor_container"];
				for (rapidjson::SizeType i = 0; i < child_scene_actor.Size(); i++)
				{
					SceneActor * child_sceneactor = CreateChildSceneActor(child_scene_actor[i], scene_actor);

					if (child_sceneactor != nullptr)
					{
						scene_actor->AddChildSceneActor(child_sceneactor);
					}
				}
			}
			
		}
		else
		{
			if (node.HasMember("path_key"))
			{
				std::string path = node["path_key"].GetString();

				if (!CheckPrefabActorFileLoaded(&m_PrefabActorFactory->GetPrefabActorContainer(), path))
				{
					m_PrefabActorFactory->LoadPrefabActorFromJsonFile(path.c_str());
				}
				prefab_actor_container_key = GetPrefabKeyByPath(&m_PrefabActorFactory->GetPrefabActorContainer(), path);

				if (prefab_actor_container_key == 0)
				{
					if (node.HasMember("object_type_key"))
					{
						std::string object_type = node["object_type_key"].GetString();
						prefab_actor_container_key = GetPrefabKeyByName(&m_PrefabActorFactory->GetPrefabActorContainer(), object_type);
						std::cout << "prefab can't be found, it will use default " << object_type << " instead." << std::endl;
					}
					else
					{
						std::cout << "cannot find \"" << path << "\" prefab, and no object type stated, it will use default prefab instead." << std::endl;
					}
				}
			}
			else
			{
				if (node.HasMember("object_type_key"))
				{
					std::string object_type = node["object_type_key"].GetString();
					prefab_actor_container_key = GetPrefabKeyByName(&m_PrefabActorFactory->GetPrefabActorContainer(), object_type);
					std::cout << "prefab can't be found, it will use default " << object_type << " instead." << std::endl;
				}
				else
				{
					std::cout << "not key path and object type given, it will use a default prefab instead." << std::endl;
				}
			}

			scene_actor = new SceneActor(m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_container_key)->second, parent_actor);

			if (node.HasMember("scene_actor_name_key"))
			{
				scene_actor->SetSceneActorName(&scene_actor_container, node["scene_actor_name_key"].GetString());
			}
			else
			{
				scene_actor->SetSceneActorName(&scene_actor_container, scene_actor->GetSceneActorName());
			}

			if (node.HasMember("active"))
			{
				scene_actor->SetOnlyActive(node["active"].GetBool());
			}
			else
			{
				scene_actor->SetOnlyActive(true);
			}

			auto comps = scene_actor->GetPrefabActor()->GetComponentsPtr();

			// some stats edit
			// old
			if (node.HasMember("m_pos.x") && node.HasMember("m_pos.y"))
			{
				if (comps->find(ComponentType::TransformComponent) != comps->end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
					trans->SetPosition(Vector2D(node["m_pos.x"].GetFloat(), node["m_pos.y"].GetFloat()));

				}
			}
			// new
			if (node.HasMember("m_Pos.x") && node.HasMember("m_Pos.y"))
			{
				if (comps->find(ComponentType::TransformComponent) != comps->end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
					trans->SetPosition(Vector2D(node["m_Pos.x"].GetFloat(), node["m_Pos.y"].GetFloat()));

				}
			}

			if (node.HasMember("m_Rotate"))
			{
				if (comps->find(ComponentType::TransformComponent) != comps->end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
					trans->SetRotate(node["m_Rotate"].GetFloat());
				}
			}
			if (node.HasMember("m_Scale.x") && node.HasMember("m_Scale.y"))
			{
				if (comps->find(ComponentType::TransformComponent) != comps->end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
					trans->SetScale(Vector3D(node["m_Scale.x"].GetFloat(), node["m_Scale.y"].GetFloat(), 0));

				}
			}
			if (node.HasMember("m_aabbScale.x") && node.HasMember("m_aabbScale.y"))
			{
				if (comps->find(ComponentType::AABBComponent) != comps->end())
				{
					AABBComponent* aabb = dynamic_cast<AABBComponent*>(comps->find(ComponentType::AABBComponent)->second);
					aabb->SetAABBScale(Vector2D(node["m_aabbScale.x"].GetFloat(), node["m_aabbScale.y"].GetFloat()));
				}
			}
			if (node.HasMember("m_offset.x") && node.HasMember("m_offset.y"))
			{
				if (comps->find(ComponentType::AABBComponent) != comps->end())
				{
					AABBComponent* aabb = dynamic_cast<AABBComponent*>(comps->find(ComponentType::AABBComponent)->second);
					aabb->SetOffset(Vector2D(node["m_offset.x"].GetFloat(), node["m_offset.y"].GetFloat()));
				}
			}
			if (node.HasMember("m_direction.x") && node.HasMember("m_direction.y"))
			{
				if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
				{
					RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
					rb->SetDirection(Vector2D(node["m_direction.x"].GetFloat(), node["m_direction.y"].GetFloat()));
				}
			}
			if (node.HasMember("m_startTime"))
			{
				if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
				{
					RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
					rb->SetStartTime(float(node["m_startTime"].GetFloat()));
				}
			}
			if (node.HasMember("m_duration"))
			{
				if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
				{
					RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
					rb->SetDuration(float(node["m_duration"].GetFloat()));
				}
			}

			if (node.HasMember("m_modA"))
			{
				if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
				{
					RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
					rb->SetModA(float(node["m_modA"].GetFloat()));
				}
			}
			if (node.HasMember("m_modB"))
			{
				if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
				{
					RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
					rb->SetModB(float(node["m_modB"].GetFloat()));
				}
			}
			if (node.HasMember("m_modC"))
			{
				if (comps->find(ComponentType::RigidBodyComponent) != comps->end())
				{
					RigidBodyComponent* rb = dynamic_cast<RigidBodyComponent*>(comps->find(ComponentType::RigidBodyComponent)->second);
					rb->SetModC(float(node["m_modC"].GetFloat()));
				}
			}


			bool non_orth = false;
			bool non_invisible = true;
			if (node.HasMember("Orthogonal_Camera"))
			{
				if (comps->find(ComponentType::SpriteComponent) != comps->end())
				{
					SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(comps->find(ComponentType::SpriteComponent)->second);
					sprite->SetOrthCam(node["Orthogonal_Camera"].GetBool());
					non_orth = !node["Orthogonal_Camera"].GetBool();
					std::string tex_name = sprite->GetSpriteName();
					if (tex_name.compare("Existence")==0)
					{
						non_invisible = false;
					}
				}
			}

			if (node.HasMember("Layer"))
			{
				if (comps->find(ComponentType::SpriteComponent) != comps->end())
				{
					SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(comps->find(ComponentType::SpriteComponent)->second);
					sprite->SetLayer(node["Layer"].GetUint());
				}
			}

			if (node.HasMember("cam_pos_x") && node.HasMember("cam_pos_y") && node.HasMember("cam_pos_z")
				&& node.HasMember("cam_front_x") && node.HasMember("cam_front_y") && node.HasMember("cam_front_z")
				&& node.HasMember("cam_up_x") && node.HasMember("cam_up_y") && node.HasMember("cam_up_z")
				&& node.HasMember("cam_offset_pos_x") && node.HasMember("cam_offset_pos_y"))
			{
				if (comps->find(ComponentType::CameraComponent) != comps->end())
				{
					CameraComponent* cam = dynamic_cast<CameraComponent*>(comps->find(ComponentType::CameraComponent)->second);
					cam->GetCamPos()->x = node["cam_pos_x"].GetFloat();
					cam->GetCamPos()->y = node["cam_pos_y"].GetFloat();
					cam->GetCamPos()->z = node["cam_pos_z"].GetFloat();

					cam->GetCamFront()->x = node["cam_front_x"].GetFloat();
					cam->GetCamFront()->y = node["cam_front_y"].GetFloat();
					cam->GetCamFront()->z = node["cam_front_z"].GetFloat();

					cam->GetCamUp()->x = node["cam_up_x"].GetFloat();
					cam->GetCamUp()->y = node["cam_up_y"].GetFloat();
					cam->GetCamUp()->z = node["cam_up_z"].GetFloat();

					*cam->GetPosOffsetX() = node["cam_offset_pos_x"].GetFloat();
					*cam->GetPosOffsetY() = node["cam_offset_pos_Y"].GetFloat();

				}
			}

			if (node.HasMember("scene_actor_tagging"))
			{
				const rapidjson::Value& tagging = node["scene_actor_tagging"];
				for (rapidjson::SizeType i = 0; i < tagging.Size(); i++)
				{
					if (tagging[i].HasMember("scene_actor_tagging_key") && tagging[i].HasMember("scene_actor_tagging_value"))
					{
						scene_actor->SetNewSceneActorTagging(tagging[i]["scene_actor_tagging_key"].GetString(), tagging[i]["scene_actor_tagging_value"].GetString());
					}
				}
			}

			if (check_level_size && non_orth && non_invisible && comps->find(ComponentType::TransformComponent) != comps->end())
			{
				TransformComponent* trans = dynamic_cast<TransformComponent*>(comps->find(ComponentType::TransformComponent)->second);
				std::cout << std::endl;
				//std::cout << scene_actor->GetSceneActorName() << " map size checked" << std::endl;
				if (first_actor_map_check)
				{
					std::cout << "first!!!" << std::endl;
					InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
					first_actor_map_check = false;
				}
				else
				{
					CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
				}
				std::cout << std::endl;
			}

			if (node.HasMember("child_scene_actor_container"))
			{
				const rapidjson::Value& child_scene_actor = node["child_scene_actor_container"];
				for (rapidjson::SizeType i = 0; i < child_scene_actor.Size(); i++)
				{
					SceneActor * child_sceneactor = CreateChildSceneActor(child_scene_actor[i], scene_actor);

					if (child_sceneactor != nullptr)
					{
						scene_actor->AddChildSceneActor(child_sceneactor);
					}
				}
			}

			
		}
	}

	return scene_actor;
}

void SceneActorFactory::SaveSceneActor(const std::string & level_name)
{
	m_PrefabActorFactory->SaveAllPrefabActor();
	//CheckAllSceneActorIntegrityCheck();

	first_actor_map_check = true;

	rapidjson::StringBuffer s;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

	writer.SetMaxDecimalPlaces(6);

	writer.StartObject();
	writer.Key("JsonFileType");
	writer.String("Level");
	
	writer.Key("SceneActor");
	writer.StartArray();
	for (const auto & elem : scene_actor_container)
	{
		writer.StartObject();
		
		bool same_data = SavePrefabIntegrityCheck(elem.second);

		if (elem.second->GetSceneActorName().compare("Surtur") == 0)
			same_data = true;

		if(elem.second->GetPrefabActor()->GetPath().compare("!")==0)
			same_data = false;

		if (same_data)
		{
			writer.Key("scene_actor_prefab_type_key");
			writer.String(elem.second->GetSceneActorPrefabType().c_str());

			writer.Key("path_key");
			writer.String(elem.second->GetPrefabActor()->GetPath().c_str());

			writer.Key("object_type_key");
			writer.String(elem.second->GetPrefabActor()->GetObjectType().c_str());

			writer.Key("scene_actor_name_key");
			writer.String(elem.second->GetSceneActorName().c_str());

			writer.Key("active");
			writer.Bool(elem.second->GetActive());

			if (elem.second->GetPrefabActor()->GetComponents().size() > 0)
			{
				auto  map = elem.second->GetPrefabActor()->GetComponents();

				if (map.find(ComponentType::TransformComponent) != map.end())
				{
					TransformComponent * trans = dynamic_cast<TransformComponent*>(map[ComponentType::TransformComponent]);
					if (map.find(ComponentType::SpriteComponent) != map.end())
					{
						SpriteComponent * sprite = dynamic_cast<SpriteComponent*>(map[ComponentType::SpriteComponent]);
						std::string tex_name = sprite->GetSpriteName();
						std::string prefab_name = elem.second->GetPrefabActor()->GetPrefabActorName();
						if (!sprite->IsOrthCam() && tex_name.compare("Existence") !=0 && prefab_name.compare("FadeToBlack")!= 0 && prefab_name.compare("MetricGrid") != 0)
						{
							std::cout << std::endl;
							//std::cout << elem.second->GetSceneActorName() << " map size checked" << std::endl;
							if (first_actor_map_check)
							{
								std::cout << "first!!!" << std::endl;
								std::cout << trans->GetPosition().x << std::endl;
								InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
								first_actor_map_check = false;
							}
							else
							{
								CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
							}
							std::cout << std::endl;
						}
					}
					writer.Key("m_Pos.x");
					writer.Double(trans->GetPosition().x);
					writer.Key("m_Pos.y");
					writer.Double(trans->GetPosition().y);
					writer.Key("m_Rotate");
					writer.Double(trans->GetRotate());
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
					writer.Key("m_offset.x");
					writer.Double(aabb->GetOffset().x);
					writer.Key("m_offset.y");
					writer.Double(aabb->GetOffset().y);
				}
				if (map.find(ComponentType::SpriteComponent) != map.end())
				{
					SpriteComponent * sprite = dynamic_cast<SpriteComponent*>(map[ComponentType::SpriteComponent]);

					writer.Key("Orthogonal_Camera");
					writer.Bool(sprite->IsOrthCam());

					writer.Key("Layer");
					writer.Uint(sprite->GetLayer());
					//if (sprite->IsOrthCam())
					//{
					//writer.Uint(sprite->GetLayer());
					//}
					//else
					//{
					//	unsigned temp = sprite->GetLayer() - 2;
					//	writer.Uint(temp);
					//}
				}
				if (map.find(ComponentType::RigidBodyComponent) != map.end())
				{
					RigidBodyComponent * rb = dynamic_cast<RigidBodyComponent*>(map[ComponentType::RigidBodyComponent]);
					writer.Key("m_direction.x");
					writer.Double(rb->GetDirection().x);
					writer.Key("m_direction.y");
					writer.Double(rb->GetDirection().y);
					writer.Key("m_startTime");
					writer.Double(rb->GetStartTime());
					writer.Key("m_duration");
					writer.Double(rb->GetDuration());
					writer.Key("m_modA");
					writer.Double(rb->GetModA());
					writer.Key("m_modB");
					writer.Double(rb->GetModB());
					writer.Key("m_modC");
					writer.Double(rb->GetModC());
				}

				if (map.find(ComponentType::CheckpointComponent) != map.end())
				{
					CheckpointComponent * checkpoint = dynamic_cast<CheckpointComponent*>(map[ComponentType::CheckpointComponent]);
					writer.Key("ComponentType");
					writer.String("CheckpointComponent");
					writer.Key("File path");
					writer.String(checkpoint->m_path.c_str());
				}


				if (map.find(ComponentType::ButtonComponent) != map.end())
				{
					ButtonComponent * button = dynamic_cast<ButtonComponent*>(map[ComponentType::ButtonComponent]);
					writer.Key("ComponentType");
					writer.String("ButtonComponent");

					writer.Key("Active Texture");
					writer.String(button->GetActiveTexName().c_str());

					writer.Key("Inactive Texture");
					writer.String(button->GetInactiveTexName().c_str());
					
					writer.Key("isHovered");
					writer.Bool(button->m_hovered);

					writer.Key("Button ID");
					writer.Int(button->m_buttonID);

					writer.Key("Trigger Type");
					writer.Int(button->m_type);

					writer.Key("Game State");
					writer.String(button->GetGameStatePath().c_str());

					writer.Key("Menu ID");
					writer.Int(button->m_menuID);

					writer.Key("isMenu");
					writer.Bool(button->m_isMenu);

					writer.Key("Next Menu");
					writer.Int(button->m_next);

					writer.Key("Section ID");
					writer.Int(button->m_sectionID);

					writer.Key("Icon");
					writer.Int(button->m_icon);

					writer.Key("Navigate");
					writer.String(button->m_navigate.c_str());

					writer.Key("Enter");
					writer.String(button->m_enter.c_str());

					writer.Key("Hovered");
					writer.Bool(button->m_enter.c_str());
				}

				if (map.find(ComponentType::DialogueComponent) != map.end())
				{
					DialogueComponent * dialogue = dynamic_cast<DialogueComponent*>(map[ComponentType::DialogueComponent]);
					writer.Key("ComponentType");
					writer.String("DialogueComponent");

					writer.Key("DialogueID");
					writer.Uint64(dialogue->m_id);

					if (dialogue->m_dialogues.size() > 0)
					{
						writer.Key("Dialogues");
						writer.StartArray();
						for (auto & text : dialogue->m_dialogues)
						{
							writer.StartObject();
							writer.Key("Dialogue Text");
							writer.String(text.c_str());
							writer.EndObject();
						}
						writer.EndArray();
					}
				}


				if (map.find(ComponentType::AudioComponent) != map.end())
				{
					AudioComponent * audio = dynamic_cast<AudioComponent*>(map[ComponentType::AudioComponent]);

					auto audioMap = audio->audioMap;

					writer.Key("ComponentType");
					writer.String("AudioComponent");
					if (audioMap.size() > 0)
					{
						writer.Key("audioMap");
						writer.StartArray();
						for (auto & elem1 : audioMap)
						{
							writer.StartObject();
							writer.Key("Path");
							writer.String(elem1.second.m_path.c_str());
							writer.Key("Volume");
							writer.Double(double(elem1.second.m_vol));
							writer.Key("Loop");
							writer.Bool(elem1.second.m_loop);
							writer.Key("State");
							writer.String(elem1.first.c_str());
							writer.EndObject();
						}
						writer.EndArray();
					}
				}


				if (map.find(ComponentType::CameraComponent) != map.end())
				{
					CameraComponent * cam = dynamic_cast<CameraComponent*>(map[ComponentType::CameraComponent]);

					writer.Key("cam_pos_x");
					writer.Double(cam->GetCamPos()->x);

					writer.Key("cam_pos_y");
					writer.Double(cam->GetCamPos()->y);

					writer.Key("cam_pos_z");
					writer.Double(cam->GetCamPos()->z);

					writer.Key("cam_front_x");
					writer.Double(cam->GetCamFront()->x);

					writer.Key("cam_front_y");
					writer.Double(cam->GetCamFront()->y);

					writer.Key("cam_front_z");
					writer.Double(cam->GetCamFront()->z);

					writer.Key("cam_up_x");
					writer.Double(cam->GetCamUp()->x);

					writer.Key("cam_up_y");
					writer.Double(cam->GetCamUp()->y);

					writer.Key("cam_up_z");
					writer.Double(cam->GetCamUp()->z);

					writer.Key("cam_offset_pos_x");
					writer.Double(*cam->GetPosOffsetX());

					writer.Key("cam_offset_pos_y");
					writer.Double(*cam->GetPosOffsetY());
				}
			}

		}
		else
		{
			writer.Key("scene_actor_prefab_type_key");
			std::string prefab_type = "NotPrefab";
			writer.String(prefab_type.c_str());

			writer.Key("scene_actor_name_key");
			writer.String(elem.second->GetSceneActorName().c_str());

			writer.Key("active");
			writer.Bool(elem.second->GetActive());

			if (elem.second->GetPrefabActor()->GetComponents().size() > 0)
			{
				writer.Key("Component");
				writer.StartArray();

				for (auto & elem2 : elem.second->GetPrefabActor()->GetComponents())
				{
					writer.StartObject();

					elem2.second->Unserialize(writer);
					
					writer.EndObject();
				}

				writer.EndArray();
			}

			

			auto  map = elem.second->GetPrefabActor()->GetComponents();
			TransformComponent * trans = dynamic_cast<TransformComponent*>(map[ComponentType::TransformComponent]);
			if (map.find(ComponentType::SpriteComponent) != map.end())
			{
				SpriteComponent * sprite = dynamic_cast<SpriteComponent*>(map[ComponentType::SpriteComponent]);
				if (!sprite->IsOrthCam())
				{
					std::cout << std::endl;
					//std::cout << elem.second->GetSceneActorName() << " map size checked" << std::endl;
					if (first_actor_map_check)
					{
						std::cout << "first!!!" << std::endl;
						InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
						first_actor_map_check = false;
					}
					else
					{
						CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
					}
					std::cout << std::endl;
				}
			}

		}

		if (elem.second->GetSceneActorTagging().size() > 2)
		{
			writer.Key("scene_actor_tagging");
			writer.StartArray();
			for (auto & elem2 : elem.second->GetSceneActorTagging())
			{
				if (elem2.first.compare("scene_actor_name_key") == 0 || elem2.first.compare("scene_actor_prefab_type_key") == 0)
					continue;
				
				writer.StartObject();
				writer.Key("scene_actor_tagging_key");
				writer.String(elem2.first.c_str());
				writer.Key("scene_actor_tagging_value");
				writer.String(elem2.second.c_str());
				writer.EndObject();
				
			}
			writer.EndArray();
		}

		if (elem.second->GetChildSceneActors().size() > 0)
		{
			writer.Key("child_scene_actor_container");
			std::cout << "child_scene_actor_container" << std::endl;
			writer.StartArray();
			for (auto & elem2 : elem.second->GetChildSceneActors())
			{
				std::cout << "a1" << std::endl;
				writer.StartObject();
				std::cout << "a2" << std::endl;
				SaveChildSceneActor(writer, elem2.second);
				std::cout << "a3" << std::endl;
				writer.EndObject();
				std::cout << "a4" << std::endl;
			}
			// convert all child prefab become scene actor prefab
			if (elem.second->GetPrefabActor()->GetChildPrefabActors().size() > 0)
			{
				for (auto & elem3 : elem.second->GetPrefabActor()->GetChildPrefabActors())
				{
					writer.StartObject();
					SaveAllNotPrefabChildPrefabInfo(writer, elem3.second);
					writer.EndObject();
				}
			}
			writer.EndArray();
		}
		else
		{
			// convert all child prefab become scene actor prefab
			if (elem.second->GetPrefabActor()->GetChildPrefabActors().size() > 0)
			{
				writer.Key("child_scene_actor_container");
				writer.StartArray();
				for (auto & elem3 : elem.second->GetPrefabActor()->GetChildPrefabActors())
				{
					writer.StartObject();
					SaveAllNotPrefabChildPrefabInfo(writer, elem3.second);
					writer.EndObject();
				}
				writer.EndArray();
			}
		}

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("LevelCoordinate");
	writer.StartObject();

	writer.Key("global_coordinate_min_x");
	writer.Double(global_coordinate_min_x);
	writer.Key("global_coordinate_max_x");
	writer.Double(global_coordinate_max_x);
	writer.Key("global_coordinate_min_y");
	writer.Double(global_coordinate_min_y);
	writer.Key("global_coordinate_max_y");
	writer.Double(global_coordinate_max_y);

	writer.EndObject();

	writer.EndObject();

	
	std::ofstream of;
	std::string path ="";
	path += "Resource/Json/Level/" + level_name + ".json";

	of.open(path.c_str());
	of << s.GetString();
	if (!of.good())
		throw std::runtime_error("Failed to save the Level!");
}

void SceneActorFactory::SaveAllNotPrefabChildPrefabInfo(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer,PrefabActor * childprefabactor)
{
	writer.Key("scene_actor_prefab_type_key");
	std::string prefab_type = "NotPrefab";
	writer.String(prefab_type.c_str());

	writer.Key("scene_actor_name_key");
	writer.String(childprefabactor->GetPrefabActorName().c_str());

	writer.Key("active");
	writer.Bool(childprefabactor->GetActive());

	if (childprefabactor->GetComponents().size() > 0)
	{
		writer.Key("Component");
		writer.StartArray();

		for (auto & elem2 : childprefabactor->GetComponents())
		{
			writer.StartObject();

			elem2.second->Unserialize(writer);

			writer.EndObject();
		}

		writer.EndArray();
	}

	if (childprefabactor->GetChildPrefabActors().size() > 0)
	{
		writer.Key("child_scene_actor_container");
		writer.StartArray();
		for (auto & elem3 : childprefabactor->GetChildPrefabActors())
		{
			writer.StartObject();
			SaveAllNotPrefabChildPrefabInfo(writer, elem3.second);
			writer.EndObject();
		}
		writer.EndArray();
	}
}

void SceneActorFactory::DeleteSceneActor(unsigned id)
{
	delete scene_actor_container[id];
	scene_actor_container[id] = nullptr;
	auto it = scene_actor_container.find(id);
	scene_actor_container.erase(it);
	m_ev->PublishEvent(new LoadLevel{});
	m_ev->PublishEvent(new SwitchLevelEvent{});
}

void SceneActorFactory::AddSceneActor(unsigned id, SceneActor * obj)
{
	
	scene_actor_container[id] = new SceneActor{ *obj };
	m_ev->PublishEvent(new LoadLevel{});
	m_ev->PublishEvent(new SwitchLevelEvent{});
}

void SceneActorFactory::SaveChildSceneActor(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer, SceneActor * child_scene_actor)
{

	bool same_data = SavePrefabIntegrityCheck(child_scene_actor);

	if (child_scene_actor->GetPrefabActor()->GetPath().compare("!") == 0)
		same_data = false;

	if (same_data)
	{
		std::cout << "b1" << std::endl;
		writer.Key("scene_actor_prefab_type_key");
		writer.String(child_scene_actor->GetSceneActorPrefabType().c_str());

		writer.Key("path_key");
		writer.String(child_scene_actor->GetPrefabActor()->GetPath().c_str());

		writer.Key("object_type_key");
		writer.String(child_scene_actor->GetPrefabActor()->GetObjectType().c_str());

		writer.Key("scene_actor_name_key");
		writer.String(child_scene_actor->GetSceneActorName().c_str());

		writer.Key("active");
		writer.Bool(child_scene_actor->GetActive());

		if (child_scene_actor->GetPrefabActor()->GetComponents().size() > 0)
		{
			auto  map = child_scene_actor->GetPrefabActor()->GetComponents();
			// small hack temp , should check what different with prefab and then will save the stuff
			if (map.find(ComponentType::TransformComponent) != map.end())
			{
				TransformComponent * trans = dynamic_cast<TransformComponent*>(map[ComponentType::TransformComponent]);
				if (map.find(ComponentType::SpriteComponent) != map.end())
				{
					SpriteComponent * sprite = dynamic_cast<SpriteComponent*>(map[ComponentType::SpriteComponent]);
					if (!sprite->IsOrthCam())
					{
						std::cout << std::endl;
						//std::cout << child_scene_actor->GetSceneActorName() << " map size checked" << std::endl;
						if (first_actor_map_check)
						{
							std::cout << "first!!!" << std::endl;
							InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
							first_actor_map_check = false;
						}
						else
						{
							CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
						}
						std::cout << std::endl;
					}
				}
				writer.Key("m_Pos.x");
				writer.Double(trans->GetPosition().x);
				writer.Key("m_Pos.y");
				writer.Double(trans->GetPosition().y);
				writer.Key("m_Rotate");
				writer.Double(trans->GetRotate());
				writer.Key("m_Scale.x");
				writer.Double(trans->GetScale().x);
				writer.Key("m_Scale.y");
				writer.Double(trans->GetScale().y);
			}

			if (map.find(ComponentType::SpriteComponent) != map.end())
			{
				SpriteComponent * sprite = dynamic_cast<SpriteComponent*>(map[ComponentType::SpriteComponent]);

				writer.Key("Orthogonal_Camera");
				writer.Bool(sprite->IsOrthCam());

				writer.Key("Layer");
				writer.Uint(sprite->GetLayer());
				//if (sprite->IsOrthCam())
				//{
				//	writer.Uint(sprite->GetLayer());
				//}
				//else
				//{
				//	unsigned temp = sprite->GetLayer() -2;
				//	writer.Uint(temp);
				//}
			}

			if (map.find(ComponentType::AABBComponent) != map.end())
			{
				AABBComponent * aabb = dynamic_cast<AABBComponent*>(map[ComponentType::AABBComponent]);
				writer.Key("m_aabbScale.x");
				writer.Double(aabb->GetAABBScale().x);
				writer.Key("m_aabbScale.y");
				writer.Double(aabb->GetAABBScale().y);
				writer.Key("m_offset.x");
				writer.Double(aabb->GetOffset().x);
				writer.Key("m_offset.y");
				writer.Double(aabb->GetOffset().y);
			}
			if (map.find(ComponentType::RigidBodyComponent) != map.end())
			{
				RigidBodyComponent * rb = dynamic_cast<RigidBodyComponent*>(map[ComponentType::RigidBodyComponent]);
				writer.Key("m_direction.x");
				writer.Double(rb->GetDirection().x);
				writer.Key("m_direction.y");
				writer.Double(rb->GetDirection().y);
				writer.Key("m_startTime");
				writer.Double(rb->GetStartTime());
				writer.Key("m_duration");
				writer.Double(rb->GetDuration());
				writer.Key("m_modA");
				writer.Double(rb->GetModA());
				writer.Key("m_modB");
				writer.Double(rb->GetModB());
				writer.Key("m_modC");
				writer.Double(rb->GetModC());
			}

			if (map.find(ComponentType::ButtonComponent) != map.end())
			{
				ButtonComponent * button = dynamic_cast<ButtonComponent*>(map[ComponentType::ButtonComponent]);
				writer.Key("ComponentType");
				writer.String("ButtonComponent");

				writer.Key("Active Texture");
				writer.String(button->GetActiveTexName().c_str());

				writer.Key("Inactive Texture");
				writer.String(button->GetInactiveTexName().c_str());

				writer.Key("Button ID");
				writer.Int(button->m_buttonID);

				writer.Key("Trigger Type");
				writer.Int(button->m_type);

				writer.Key("Section ID");
				writer.Int(button->m_sectionID);

				writer.Key("Game State");
				writer.String(button->GetGameStatePath().c_str());

				writer.Key("Menu ID");
				writer.Int(button->m_menuID);

				writer.Key("isMenu");
				writer.Bool(button->m_isMenu);

				writer.Key("Next Menu");
				writer.Int(button->m_next);

				writer.Key("Navigate");
				writer.String(button->m_navigate.c_str());

				writer.Key("Enter");
				writer.String(button->m_enter.c_str());
			}


			if (map.find(ComponentType::AudioComponent) != map.end())
			{
				AudioComponent * audio = dynamic_cast<AudioComponent*>(map[ComponentType::AudioComponent]);

				auto audioMap = audio->audioMap;

				writer.Key("ComponentType");
				writer.String("AudioComponent");
				if (audioMap.size() > 0)
				{
					writer.Key("audioMap");
					writer.StartArray();
					for (auto & elem1 : audioMap)
					{
						writer.StartObject();
						writer.Key("Path");
						writer.String(elem1.second.m_path.c_str());
						writer.Key("Volume");
						writer.Double(double(elem1.second.m_vol));
						writer.Key("Loop");
						writer.Bool(elem1.second.m_loop);
						writer.Key("State");
						writer.String(elem1.first.c_str());
						writer.EndObject();
					}
					writer.EndArray();
				}
			}


			if (map.find(ComponentType::CameraComponent) != map.end())
			{
				CameraComponent * cam = dynamic_cast<CameraComponent*>(map[ComponentType::CameraComponent]);

				writer.Key("cam_pos_x");
				writer.Double(cam->GetCamPos()->x);

				writer.Key("cam_pos_y");
				writer.Double(cam->GetCamPos()->y);

				writer.Key("cam_pos_z");
				writer.Double(cam->GetCamPos()->z);

				writer.Key("cam_front_x");
				writer.Double(cam->GetCamFront()->x);

				writer.Key("cam_front_y");
				writer.Double(cam->GetCamFront()->y);

				writer.Key("cam_front_z");
				writer.Double(cam->GetCamFront()->z);

				writer.Key("cam_up_x");
				writer.Double(cam->GetCamUp()->x);

				writer.Key("cam_up_y");
				writer.Double(cam->GetCamUp()->y);

				writer.Key("cam_up_z");
				writer.Double(cam->GetCamUp()->z);

				writer.Key("cam_offset_pos_x");
				writer.Double(*cam->GetPosOffsetX());

				writer.Key("cam_offset_pos_y");
				writer.Double(*cam->GetPosOffsetY());
			}
		}
		std::cout << "b2" << std::endl;
		
	}
	else
	{
	   
		writer.Key("scene_actor_prefab_type_key");
		std::string prefab_type = "NotPrefab";
		writer.String(prefab_type.c_str());

		writer.Key("scene_actor_name_key");
		writer.String(child_scene_actor->GetSceneActorName().c_str());

		writer.Key("active");
		writer.Bool(child_scene_actor ->GetActive());

		if (child_scene_actor->GetPrefabActor()->GetComponents().size() > 0)
		{
			writer.Key("Component");
			writer.StartArray();

			for (auto & elem2 : child_scene_actor->GetPrefabActor()->GetComponents())
			{
				writer.StartObject();

				elem2.second->Unserialize(writer);

				writer.EndObject();
			}

			writer.EndArray();
		}

		auto  map = child_scene_actor->GetPrefabActor()->GetComponents();
		TransformComponent * trans = dynamic_cast<TransformComponent*>(map[ComponentType::TransformComponent]);
		if (map.find(ComponentType::SpriteComponent) != map.end())
		{
			SpriteComponent * sprite = dynamic_cast<SpriteComponent*>(map[ComponentType::SpriteComponent]);
			std::string tex_name = sprite->GetSpriteName();
			std::string prefab_name = child_scene_actor->GetPrefabActor()->GetPrefabActorName();
			if (!sprite->IsOrthCam() && tex_name.compare("Existence") != 0 && prefab_name.compare("FadeToBlack") != 0 && prefab_name.compare("MetricGrid") != 0)
			{
				std::cout << std::endl;
				//std::cout << child_scene_actor->GetSceneActorName() << " map size checked" << std::endl;
				if (first_actor_map_check)
				{
					std::cout << "first!!!" << std::endl;
					InitGlobalCoordinate(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
					first_actor_map_check = false;
				}
				else
				{
					CalculateLevelSize(trans->GetPosition().x, trans->GetPosition().y, trans->GetScale().x, trans->GetScale().y, trans->GetRotate());
				}
				std::cout << std::endl;
			}
		}
	}

	if (child_scene_actor->GetSceneActorTagging().size() > 1)
	{
		writer.Key("scene_actor_tagging");
		writer.StartArray();
		for (auto & elem2 : child_scene_actor->GetSceneActorTagging())
		{
			if (elem2.first.compare("scene_actor_name_key") != 0)
			{
				writer.StartObject();
				writer.Key("scene_actor_tagging_key");
				writer.String(elem2.first.c_str());
				writer.Key("scene_actor_tagging_value");
				writer.String(elem2.second.c_str());
				writer.EndObject();
			}
		}
		writer.EndArray();
	}
	std::cout << "b3" << std::endl;
	if (child_scene_actor->GetChildSceneActors().size() > 0)
	{
		writer.Key("child_scene_actor_container");
		writer.StartArray();
		for (auto & elem2 : child_scene_actor->GetChildSceneActors())
		{
			writer.StartObject();
		
			SaveChildSceneActor(writer, elem2.second);
		
			writer.EndObject();
		}
		// convert all child prefab become scene actor prefab
		if (child_scene_actor->GetPrefabActor()->GetChildPrefabActors().size() > 0)
		{
			for (auto & elem3 : child_scene_actor->GetPrefabActor()->GetChildPrefabActors())
			{
				writer.StartObject();
				SaveAllNotPrefabChildPrefabInfo(writer, elem3.second);
				writer.EndObject();
			}
		}
		writer.EndArray();
	}
	else
	{
		if (child_scene_actor->GetPrefabActor()->GetChildPrefabActors().size() > 0)
		{
			writer.Key("child_scene_actor_container");
			writer.StartArray();
			for (auto & elem3 : child_scene_actor->GetPrefabActor()->GetChildPrefabActors())
			{
				writer.StartObject();
				SaveAllNotPrefabChildPrefabInfo(writer, elem3.second);
				writer.EndObject();
			}
			writer.EndArray();
		}
		
	}
}


std::map<unsigned, SceneActor *> & SceneActorFactory::GetSceneActorContainer()
{
	return scene_actor_container;
}

std::map<unsigned, SceneActor *> * SceneActorFactory::GetLoadingContainer()
{
	return &loading_container;
}


void SceneActorFactory::UnloadSceneActorFactory()
{
	current_scene_camera = nullptr;
	for (auto & elem : scene_actor_container)
	{
		delete elem.second;
		elem.second = nullptr;
	}
	scene_actor_container.clear();
	scene_actor_container_key = 1;

	m_ev->PublishEvent(new SwitchLevelEvent{});
	m_ev->PublishEvent(new LoadLevel{});
}


void SceneActorFactory::RestartSceneLevel()
{
	m_ev->PublishEvent(new SwitchLevelEvent{});
	m_ev->PublishEvent(new LoadLevel{});
	m_ev->PublishEvent(new PlayLevel{});
}
SceneActor * SceneActorFactory::NewSceneActor(unsigned key, SceneActor * added_target)
{
	std::cout << "created prefab key:"<< key << std::endl;

	SceneActor * new_scene_actor = new SceneActor(m_PrefabActorFactory->GetPrefabActorContainer().find(key)->second);

	auto comp = new_scene_actor->GetPrefabActor()->GetComponents();

	if (comp.find(ComponentType::TransformComponent) != comp.end())
	{
		TransformComponent* trans = dynamic_cast<TransformComponent*>(comp.find(ComponentType::TransformComponent)->second);

		std::cout << trans->GetPosition() << std::endl;
	}
	
	if (new_scene_actor->GetPrefabActor()->GetPath().size() <= 0)
	{
		std::cout << "should not run"<< std::endl;
		auto default_prefab_container = m_PrefabActorFactory->GetDefaultPrefabActorContainer();

		if (default_prefab_container.find(new_scene_actor->GetPrefabActor()->GetPrefabActorName()) != default_prefab_container.end())
		{
			std::string name = default_prefab_container.find(new_scene_actor->GetPrefabActor()->GetPrefabActorName())->second;
			std::string tmp_path = "Resource/Json/PrefabActor/" + name + ".json";
			new_scene_actor->GetPrefabActor()->SetPath(tmp_path);
			new_scene_actor->GetPrefabActor()->SetPrefabActorName(&m_PrefabActorFactory->GetPrefabActorContainer(), name);
			new_scene_actor->SetSceneActorName(new_scene_actor->GetPrefabActor()->GetPrefabActorName());
		}
		else
		{
			std::string tmp_path = "Resource/Json/PrefabActor/New Prefab.json";
			new_scene_actor->GetPrefabActor()->SetPath(tmp_path);
			new_scene_actor->GetPrefabActor()->SetPrefabActorName(&m_PrefabActorFactory->GetPrefabActorContainer(), "New Prefab");
			new_scene_actor->SetSceneActorName(new_scene_actor->GetPrefabActor()->GetPrefabActorName());
		}
	}

	if (added_target == nullptr)
	{
		new_scene_actor->SetSceneActorName(&GetSceneActorContainer(), new_scene_actor->GetSceneActorName());
		/*unsigned count = 1;
		while (scene_actor_container.find(count) != scene_actor_container.end())
		{
			++count;
		}*/
		scene_actor_container.emplace(scene_actor_container_key, new_scene_actor);
		++scene_actor_container_key;
	}
	else
	{
		
		auto parent_comp = added_target->GetPrefabActor()->GetComponents();
		auto child_comp = new_scene_actor->GetPrefabActor()->GetComponents();
		if (parent_comp.find(ComponentType::TransformComponent) != parent_comp.end() && child_comp.find(ComponentType::TransformComponent) != child_comp.end())
		{
			TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp.find(ComponentType::TransformComponent)->second);
			TransformComponent* trans = dynamic_cast<TransformComponent*>(child_comp.find(ComponentType::TransformComponent)->second);
			trans->SetParentTransformComponent(parent_trans);
		}
		new_scene_actor->SetParentSceneActorPointer(added_target);
		added_target->AddChildSceneActor(new_scene_actor);
	}
	
	m_ev->PublishEvent(new LoadLevel{});
	m_ev->PublishEvent(new SwitchLevelEvent{});
	return new_scene_actor;
}

void SceneActorFactory::CloneSceneActor(SceneActor * clone_object)
{
	SceneActor * new_scene_actor = new SceneActor(*clone_object);
	new_scene_actor->SetSceneActorName(&GetSceneActorContainer(), new_scene_actor->GetSceneActorName());
	/*unsigned count = 1;
	while (scene_actor_container.find(count) != scene_actor_container.end())
	{
		++count;
	}*/
	scene_actor_container.emplace(scene_actor_container_key, new_scene_actor);
	++scene_actor_container_key;

	m_ev->PublishEvent(new SwitchLevelEvent{});
	m_ev->PublishEvent(new LoadLevel{});
}

void SceneActorFactory::SceneActorSelectionDuplicate()
{
	CheckAllSceneActorIntegrityCheck();
	for (auto & elem : scene_actor_container)
	{
		if (elem.second->GetSceneActorSelection()==true)
		{
			SceneActor * duplicate_scene_actor = new SceneActor(*(elem.second), false);
			duplicate_scene_actor->SetSceneActorName(&scene_actor_container, duplicate_scene_actor->GetSceneActorName());
			/*unsigned count = 1;
			while (scene_actor_container.find(count) != scene_actor_container.end())
			{
				++count;
			}*/
			scene_actor_container.emplace(scene_actor_container_key, duplicate_scene_actor);
			++scene_actor_container_key;
			for (auto & childs_elem : elem.second->GetChildSceneActors())
			{
				ChildSceneActorSelectionDuplicate(childs_elem.second, duplicate_scene_actor, true);
			}
		
		}
		else
		{
			for (auto & childs_elem : elem.second->GetChildSceneActors())
				ChildSceneActorSelectionDuplicate(childs_elem.second,elem.second);
		}
	}

	m_ev->PublishEvent(new SwitchLevelEvent{});
	m_ev->PublishEvent(new LoadLevel{});
}

void SceneActorFactory::ChildSceneActorSelectionDuplicate(SceneActor * copy_source, SceneActor * dest_source, bool duplicate_container)
{
	if (copy_source->GetSceneActorSelection() == true)
	{
		SceneActor * duplicate_scene_actor = new SceneActor(*(copy_source), false);
		duplicate_scene_actor->SetParentSceneActorPointer(dest_source);
		auto parent_map = dest_source->GetPrefabActor()->GetComponents();
		auto child_map = duplicate_scene_actor->GetPrefabActor()->GetComponents();
		if (parent_map.find(ComponentType::TransformComponent) != parent_map.end() && child_map.find(ComponentType::TransformComponent) != child_map.end())
		{
			TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_map.find(ComponentType::TransformComponent)->second);
			TransformComponent* trans = dynamic_cast<TransformComponent*>(child_map.find(ComponentType::TransformComponent)->second);
			trans->SetParentTransformComponent(parent_trans);
		}
		dest_source->AddChildSceneActor(duplicate_scene_actor);
		for (auto & childs_elem : copy_source->GetChildSceneActors())
		{
			ChildSceneActorSelectionDuplicate(childs_elem.second, duplicate_scene_actor, duplicate_container);
		}
	}
	else
	{
		if (duplicate_container)
		{
			for (auto & childs_elem : copy_source->GetChildSceneActors())
			{
				ChildSceneActorSelectionDuplicate(childs_elem.second, dest_source, duplicate_container);
			}
		}
		else
		{
			for (auto & childs_elem : copy_source->GetChildSceneActors())
			{
				ChildSceneActorSelectionDuplicate(childs_elem.second, copy_source, duplicate_container);
			}
		}
	}
}

void SceneActorFactory::SceneActorSelectionDelete()
{
	CheckAllSceneActorIntegrityCheck();
	for (auto it = scene_actor_container.begin(); it != scene_actor_container.end(); )
	{
		
		ChildSceneActorSelectionDelete(it->second->GetChildSceneActors());
		
		if (it->second->GetSceneActorSelection() == true)
		{
			// check got any child left, if yes need back up
			if (it->second->GetChildSceneActors().size() > 0)
			{
				// find the correct container to back up
				// will check above layer it is also going deleted
				// if not use it, if yes go above layer again until null
				// if null mean just back up at the first layer
				SceneActor* find_container = it->second->GetParentSceneActorPointer();
				while (find_container != nullptr && find_container->GetSceneActorSelection() == true)
				{
					find_container = find_container->GetParentSceneActorPointer();
				}
				// get the key number to add, in my stl container all key value start at 1, 0 is for not found
				//unsigned count = 1;
				for (auto & elem : it->second->GetChildSceneActors())
				{
					if (find_container == nullptr)
					{
						/*while (scene_actor_container.find(count) != scene_actor_container.end())
						{
							++count;
						}*/
						// change pointer to new container
						scene_actor_container.emplace(scene_actor_container_key, elem.second);
						++scene_actor_container_key;
						elem.second = nullptr;
						//++count;
					}
					else
					{
						/*while (find_container->GetChildSceneActors().find(count) != find_container->GetChildSceneActors().end())
						{
							++count;
						}*/
						// relink all pointer information
						auto parent_map = find_container->GetPrefabActor()->GetComponents();
						auto child_map = elem.second->GetPrefabActor()->GetComponents();
						if (parent_map.find(ComponentType::TransformComponent) != parent_map.end() && child_map.find(ComponentType::TransformComponent) != child_map.end())
						{
							TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_map.find(ComponentType::TransformComponent)->second);
							TransformComponent* trans = dynamic_cast<TransformComponent*>(child_map.find(ComponentType::TransformComponent)->second);
							trans->SetParentTransformComponent(parent_trans);
						}
						elem.second->SetParentSceneActorPointer(find_container);
						// change pointer to new container
						
						find_container->GetChildSceneActors().emplace(*find_container->GetCurrentKeyIDPtr(), elem.second);
						++(*find_container->GetCurrentKeyIDPtr());
						elem.second = nullptr;
						//++count;
					}
				}
				// clear the child container, no delete require as move the pointer content
				it->second->GetChildSceneActors().clear();
				// unlink all the pointer info at this actor, set all to null
				auto comp_map = it->second->GetPrefabActor()->GetComponents();
				if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(nullptr);
				}
				it->second->SetParentSceneActorPointer(nullptr);
				// delete and erase;
				delete it->second;
				it->second = nullptr;
				it = scene_actor_container.erase(it);

			}
			else
			{
				// mean no child, can delink all pointer and delete
				auto comp_map = it->second->GetPrefabActor()->GetComponents();
				if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(nullptr);
				}
				it->second->SetParentSceneActorPointer(nullptr);
				delete it->second;
				it->second = nullptr;
				it = scene_actor_container.erase(it);
			}
		}
		else
		{
			++it;
		}
	}

	m_ev->PublishEvent(new SwitchLevelEvent{});
	m_ev->PublishEvent(new LoadLevel{});
	
}

void SceneActorFactory::ChildSceneActorSelectionDelete(std::map<unsigned, SceneActor *> & container)
{
	for (auto it = container.begin(); it != container.end();  )
	{
		// check the child layer first recursive
		ChildSceneActorSelectionDelete(it->second->GetChildSceneActors());

		// check if selected or not, selected = delete
		if (it->second->GetSceneActorSelection()== true)
		{
			// check got any child left, if yes need back up
			if (it->second->GetChildSceneActors().size() > 0)
			{
				// find the correct container to back up
				// will check above layer it is also going deleted
				// if not use it, if yes go above layer again until null
				// if null mean just back up at the first layer
				SceneActor* find_container = it->second->GetParentSceneActorPointer();
				while (find_container != nullptr && find_container->GetSceneActorSelection() == true)
				{
					find_container = find_container->GetParentSceneActorPointer();
				}
				// get the key number to add, in my stl container all key value start at 1, 0 is for not found
				//unsigned count = 1;
				for (auto & elem : it->second->GetChildSceneActors())
				{
					if (find_container == nullptr)
					{
						/*while (scene_actor_container.find(count) != scene_actor_container.end())
						{
							++count;
						}*/
						// change pointer to new container
						scene_actor_container.emplace(scene_actor_container_key, elem.second);
						++scene_actor_container_key;
						elem.second = nullptr;
						//++count;
					}
					else
					{
						/*while (find_container->GetChildSceneActors().find(count) != find_container->GetChildSceneActors().end())
						{
							++count;
						}*/
						// relink all pointer information
						auto parent_map = find_container->GetPrefabActor()->GetComponents();
						auto child_map = elem.second->GetPrefabActor()->GetComponents();
						if (parent_map.find(ComponentType::TransformComponent) != parent_map.end() && child_map.find(ComponentType::TransformComponent) != child_map.end())
						{
							TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_map.find(ComponentType::TransformComponent)->second);
							TransformComponent* trans = dynamic_cast<TransformComponent*>(child_map.find(ComponentType::TransformComponent)->second);
							trans->SetParentTransformComponent(parent_trans);
						}
						elem.second->SetParentSceneActorPointer(find_container);
						// change pointer to new container
						find_container->GetChildSceneActors().emplace(*find_container->GetCurrentKeyIDPtr(), elem.second);
						++(*find_container->GetCurrentKeyIDPtr());
						elem.second = nullptr;
						//++count;
					}
				}
				// clear the child container, no delete require as move the pointer content
				it->second->GetChildSceneActors().clear();
				// unlink all the pointer info at this actor, set all to null
				auto comp_map = it->second->GetPrefabActor()->GetComponents();
				if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(nullptr);
				}
				it->second->SetParentSceneActorPointer(nullptr);
				// delete and erase;
				delete it->second;
				it->second = nullptr;
				it = container.erase(it);

			}
			else
			{
				// mean no child, can delink all pointer and delete
				auto comp_map = it->second->GetPrefabActor()->GetComponents();
				if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(nullptr);
				}
				it->second->SetParentSceneActorPointer(nullptr);
				delete it->second;
				it->second = nullptr;
				it = container.erase(it);
			}
		}
		else
		{
			++it;


		}
	}
}

void SceneActorFactory::SceneActorSelectionMove(SceneActor * dest)
{
	std::cout << "test run" << std::endl;
	CheckAllSceneActorIntegrityCheck();
	if (dest == nullptr)
	{
		AttachAllSelectionToDestSceneActor(dest);
	}
	else
	{
		SceneActor * check_parent_ptr = dest;
		SceneActor * check_child_ptr = dest;
		while (check_parent_ptr != nullptr)
		{
			check_parent_ptr = check_parent_ptr->GetParentSceneActorPointer();
			if (check_parent_ptr != nullptr)
			{
				auto container = check_parent_ptr->GetChildSceneActorsPtr();
				unsigned container_key = GetSceneActorKeyByName(container, check_child_ptr->GetSceneActorName());
				container_index.push_front(container_key);
				check_child_ptr = check_parent_ptr;
			}
		}
		while (check_child_ptr != dest)
		{
			if (check_child_ptr->GetSceneActorSelection() == true)
			{
				SceneActor * old_parent = dest->GetParentSceneActorPointer();
				unsigned old_container_key = GetSceneActorKeyByName(old_parent->GetChildSceneActorsPtr(), dest->GetSceneActorName());
				// delink all pointer
				auto comp_map = dest->GetPrefabActor()->GetComponents();
				if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
				{
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(nullptr);
				}
				if (check_parent_ptr == nullptr)
				{
					dest->SetParentSceneActorPointer(nullptr);
					/*unsigned count = 1;
					while (scene_actor_container.find(count) != scene_actor_container.end())
					{
						++count;
					}*/
					dest->SetSceneActorName(&scene_actor_container, dest->GetSceneActorName());
					scene_actor_container.emplace(scene_actor_container_key, dest);
					++scene_actor_container_key;
					old_parent->GetChildSceneActors().find(old_container_key)->second = nullptr;
					old_parent->GetChildSceneActors().erase(old_container_key);
					old_parent = nullptr;
				}
				else
				{
					auto parent_comp_map = check_parent_ptr->GetPrefabActor()->GetComponents();
					if (parent_comp_map.find(ComponentType::TransformComponent) != parent_comp_map.end() && comp_map.find(ComponentType::TransformComponent) != comp_map.end())
					{
						TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp_map.find(ComponentType::TransformComponent)->second);
						TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
						trans->SetParentTransformComponent(parent_trans);
					}
					dest->SetParentSceneActorPointer(check_parent_ptr);
					check_parent_ptr->AddChildSceneActor(dest);
					old_parent->GetChildSceneActors().find(old_container_key)->second = nullptr;
					old_parent->GetChildSceneActors().erase(old_container_key);
					old_parent = nullptr;
				}
				check_parent_ptr = nullptr;
				check_child_ptr = nullptr;
				container_index.clear();
				AttachAllSelectionToDestSceneActor(dest);
			}
			check_parent_ptr = check_child_ptr;
			auto child_container = check_child_ptr->GetChildSceneActorsPtr();
			check_child_ptr = child_container->find(container_index.front())->second;
			container_index.pop_front();
		}
		// all parent not been selected process next stage
		check_parent_ptr = nullptr;
		check_child_ptr = nullptr;
		container_index.clear();
		AttachAllSelectionToDestSceneActor(dest);
	}
}
void SceneActorFactory::AttachAllSelectionToDestSceneActor(SceneActor * dest)
{
	for (auto  it = scene_actor_container.begin() ; it != scene_actor_container.end();)
	{
		if (it->second->GetChildSceneActors().size() > 0)
		{
			auto child_map = it->second->GetChildSceneActors();
			for (auto & child_elem : child_map)
			{
				AttachAllSelectionToDestSceneActor(dest, child_elem.second);
			}
		}
		
		if (dest != nullptr &&  dest != it->second && it->second->GetSceneActorSelection() ==true)
		{
			auto parent_map = dest->GetPrefabActor()->GetComponents();
			auto child_map = it->second->GetPrefabActor()->GetComponents();
			if (parent_map.find(ComponentType::TransformComponent) != parent_map.end() && child_map.find(ComponentType::TransformComponent) != child_map.end())
			{
				TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_map.find(ComponentType::TransformComponent)->second);
				TransformComponent* trans = dynamic_cast<TransformComponent*>(child_map.find(ComponentType::TransformComponent)->second);
				trans->SetParentTransformComponent(parent_trans);
			}
			it->second->SetParentSceneActorPointer(dest);
			dest->AddChildSceneActor(it->second);
			it->second = nullptr;
			it = scene_actor_container.erase(it);
		}
		else
		{
			++it;
		}	
	}
}

void SceneActorFactory::AttachAllSelectionToDestSceneActor(SceneActor * dest, SceneActor * check_actor)
{
	
	if (check_actor->GetChildSceneActors().size() > 0)
	{
		// recurrsive check through all all object
		auto child_map = check_actor->GetChildSceneActors();
		for (auto & child_elem : child_map)
		{
			AttachAllSelectionToDestSceneActor(dest, child_elem.second);
		}
	}

	if (check_actor->GetSceneActorSelection() == true && check_actor != dest)
	{
		SceneActor * old_parent = check_actor->GetParentSceneActorPointer();
		if (old_parent !=  nullptr )
		{
			auto comp_map = check_actor->GetPrefabActor()->GetComponents();
			if (comp_map.find(ComponentType::TransformComponent) != comp_map.end())
			{
				TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
				trans->SetParentTransformComponent(nullptr);
			}
			unsigned old_container_key = GetSceneActorKeyByName(old_parent->GetChildSceneActorsPtr(), check_actor->GetSceneActorName());
			if (dest == nullptr)
			{
				check_actor->SetParentSceneActorPointer(nullptr);
				/*unsigned count = 1;
				while (scene_actor_container.find(count) != scene_actor_container.end())
				{
					++count;
				}*/
				check_actor->SetSceneActorName(&scene_actor_container, check_actor->GetSceneActorName());
				scene_actor_container.emplace(scene_actor_container_key, check_actor);
				++scene_actor_container_key;
				old_parent->GetChildSceneActors().find(old_container_key)->second = nullptr;
				old_parent->GetChildSceneActors().erase(old_container_key);
				old_parent = nullptr;
			}
			else
			{
				auto parent_comp_map = dest->GetPrefabActor()->GetComponents();
				if (parent_comp_map.find(ComponentType::TransformComponent) != parent_comp_map.end() && comp_map.find(ComponentType::TransformComponent) != comp_map.end())
				{
					TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp_map.find(ComponentType::TransformComponent)->second);
					TransformComponent* trans = dynamic_cast<TransformComponent*>(comp_map.find(ComponentType::TransformComponent)->second);
					trans->SetParentTransformComponent(parent_trans);
				}
				check_actor->SetParentSceneActorPointer(dest);
				dest->AddChildSceneActor(check_actor);
				old_parent->GetChildSceneActors().find(old_container_key)->second = nullptr;
				old_parent->GetChildSceneActors().erase(old_container_key);
				old_parent = nullptr;
			}
		}
		
	}
}

void SceneActorFactory::ClearWholeSceneActorSelection()
{
	for (auto & elem : scene_actor_container)
	{
		elem.second->ClearSceneActorSelection();
	}
}

void SceneActorFactory::SceneActorSelectionRename(std::string name)
{
	//CheckAllSceneActorIntegrityCheck();
	for (auto & elem : scene_actor_container)
	{
		if (elem.second->GetSceneActorSelection())
		{
			elem.second->SetSceneActorName(&scene_actor_container, name);
		}

		if (elem.second->GetChildSceneActors().size() > 0)
		{
			for (auto & child_elem : elem.second->GetChildSceneActors())
			{
				SceneActorSelectionRename(name, child_elem.second);
			}
		}
	}
}

void SceneActorFactory::SceneActorSelectionRename(std::string name, SceneActor * actor)
{
	if (actor->GetSceneActorSelection())
	{
		actor->SetSceneActorName(&actor->GetParentSceneActorPointer()->GetChildSceneActors(), name);
	}
	
	if (actor->GetChildSceneActors().size() > 0)
	{
		for (auto & child_elem : actor->GetChildSceneActors())
		{
			SceneActorSelectionRename(name, child_elem.second);
		}
	}
}

void SceneActorFactory::UpdateSceneActorCriticalInfo(std::string path_to_search, PrefabActor * update_info)
{
	CheckAllSceneActorIntegrityCheck();
	if (scene_actor_container.size() > 0)
	{
		for (auto & elem : scene_actor_container)
		{
			if (elem.second->GetPrefabActor()->GetPath().compare(path_to_search) == 0)
			{
				elem.second->GetPrefabActor()->SetPath(update_info->GetPath());
				elem.second->GetPrefabActor()->SetFileName(update_info->GetFilename());
				elem.second->GetPrefabActor()->SetPrefabActorName(update_info->GetPrefabActorName());
			}

			if (elem.second->GetChildSceneActors().size() > 0)
			{
				for (auto & child_elem : elem.second->GetChildSceneActors())
				{
					UpdateSceneActorCriticalInfo(path_to_search, update_info, child_elem.second);
				}
			}
		}
	}
}

void SceneActorFactory::UpdateSceneActorCriticalInfo(std::string path_to_search, PrefabActor * update_info, SceneActor * actor)
{
	if (actor->GetPrefabActor()->GetPath().compare(path_to_search) == 0)
	{
		actor->GetPrefabActor()->SetPath(update_info->GetPath());
		actor->GetPrefabActor()->SetFileName(update_info->GetFilename());
		actor->GetPrefabActor()->SetPrefabActorName(update_info->GetPrefabActorName());
	}

	if (actor->GetChildSceneActors().size() > 0)
	{
		for (auto & child_elem : actor->GetChildSceneActors())
		{
			UpdateSceneActorCriticalInfo(path_to_search, update_info, child_elem.second);
		}
	}
}
void SceneActorFactory::UpdateAllSceneActor(PrefabActor * update_info)
{
	//CheckAllSceneActorIntegrityCheck();
	if (scene_actor_container.size() > 0)
	{
		for (auto & elem : scene_actor_container)
		{
			UpdateSceneActor(update_info, elem.second);
		}
		m_ev->PublishEvent(new SwitchLevelEvent());
		m_ev->PublishEvent(new LoadLevel{});
	}
}

void SceneActorFactory::UpdateSceneActor(PrefabActor * update_info , SceneActor * actor)
{
	if (actor->GetPrefabActor()->GetPath().compare(update_info->GetPath()) == 0)
	{
		PrefabActor * new_prefab_info = new PrefabActor(*update_info);
		PrefabActor * old_prefab_info = actor->GetPrefabActor();
		actor->SetPrefabActor(new_prefab_info);

		auto new_prefab_comp = actor->GetPrefabActor()->GetComponents();
		auto old_prefab_comp = old_prefab_info->GetComponents();

		if (new_prefab_comp.find(ComponentType::TransformComponent) != new_prefab_comp.end() && old_prefab_comp.find(ComponentType::TransformComponent) != old_prefab_comp.end())
		{
			TransformComponent* new_trans = dynamic_cast<TransformComponent*>(new_prefab_comp.find(ComponentType::TransformComponent)->second);
			TransformComponent* old_trans = dynamic_cast<TransformComponent*>(old_prefab_comp.find(ComponentType::TransformComponent)->second);
			new_trans->SetPosition(old_trans->GetPosition());
			new_trans->SetScale(old_trans->GetScale());
		}

		if (new_prefab_comp.find(ComponentType::AABBComponent) != new_prefab_comp.end() && old_prefab_comp.find(ComponentType::AABBComponent) != old_prefab_comp.end())
		{
			AABBComponent* new_aabb = dynamic_cast<AABBComponent*>(new_prefab_comp.find(ComponentType::AABBComponent)->second);
			AABBComponent* old_aabb = dynamic_cast<AABBComponent*>(old_prefab_comp.find(ComponentType::AABBComponent)->second);
			new_aabb->SetAABBScale(old_aabb->GetAABBScale());
		}

		if (actor->GetChildSceneActors().size() > 0)
		{
			for (auto child_elem : actor->GetChildSceneActors())
			{
				UpdateSceneActor(update_info, actor,child_elem.second);
			}
		}
		delete old_prefab_info;
	}
	else
	{
		if (actor->GetChildSceneActors().size() > 0)
		{
			for (auto child_elem : actor->GetChildSceneActors())
			{
				UpdateSceneActor(update_info, actor, child_elem.second);
			}
		}
	}	
}

void SceneActorFactory::UpdateSceneActor(PrefabActor * update_info, SceneActor * parent, SceneActor * actor)
{
	// also need update pointer incase parent actually changed
	auto parent_comp_map = parent->GetPrefabActor()->GetComponents();
	auto child_comp_map = actor->GetPrefabActor()->GetComponents();
	if (parent_comp_map.find(ComponentType::TransformComponent) != parent_comp_map.end() && child_comp_map.find(ComponentType::TransformComponent) != child_comp_map.end())
	{
		TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp_map.find(ComponentType::TransformComponent)->second);
		TransformComponent* child_trans = dynamic_cast<TransformComponent*>(child_comp_map.find(ComponentType::TransformComponent)->second);
		child_trans->SetParentTransformComponent(parent_trans);
	}

	if (actor->GetPrefabActor()->GetPath().compare(update_info->GetPath()) == 0)
	{
		PrefabActor * new_prefab_info = new PrefabActor(*update_info);
		PrefabActor * old_prefab_info = actor->GetPrefabActor();
		auto old_prefab_comp = old_prefab_info->GetComponents();

		// unlink the transform pointer
		if (old_prefab_comp.find(ComponentType::TransformComponent) != old_prefab_comp.end())
		{
			TransformComponent* old_trans = dynamic_cast<TransformComponent*>(old_prefab_comp.find(ComponentType::TransformComponent)->second);
			old_trans->SetParentTransformComponent(nullptr);
		}
		// swap pointer
		actor->SetPrefabActor(new_prefab_info);
		auto new_prefab_comp = actor->GetPrefabActor()->GetComponents();

		// set back the transform value 
		if (new_prefab_comp.find(ComponentType::TransformComponent) != new_prefab_comp.end() && old_prefab_comp.find(ComponentType::TransformComponent) != old_prefab_comp.end())
		{
			TransformComponent* new_trans = dynamic_cast<TransformComponent*>(new_prefab_comp.find(ComponentType::TransformComponent)->second);
			TransformComponent* old_trans = dynamic_cast<TransformComponent*>(old_prefab_comp.find(ComponentType::TransformComponent)->second);
			new_trans->SetPosition(old_trans->GetPosition());
			new_trans->SetScale(old_trans->GetScale());
		}

		// link the parent transform
		auto parent_comp = parent->GetPrefabActor()->GetComponents();
		if (new_prefab_comp.find(ComponentType::TransformComponent) != new_prefab_comp.end() && parent_comp.find(ComponentType::TransformComponent) != parent_comp.end())
		{
			TransformComponent* parent_trans = dynamic_cast<TransformComponent*>(parent_comp.find(ComponentType::TransformComponent)->second);
			TransformComponent* new_trans = dynamic_cast<TransformComponent*>(new_prefab_comp.find(ComponentType::TransformComponent)->second);
			new_trans->SetParentTransformComponent(parent_trans);
		}

		// set back the aabb value
		if (new_prefab_comp.find(ComponentType::AABBComponent) != new_prefab_comp.end() && old_prefab_comp.find(ComponentType::AABBComponent) != old_prefab_comp.end())
		{
			AABBComponent* new_aabb = dynamic_cast<AABBComponent*>(new_prefab_comp.find(ComponentType::AABBComponent)->second);
			AABBComponent* old_aabb = dynamic_cast<AABBComponent*>(old_prefab_comp.find(ComponentType::AABBComponent)->second);
			new_aabb->SetAABBScale(old_aabb->GetAABBScale());
		}
		// check is there child
		if (actor->GetChildSceneActors().size() > 0)
		{
			for (auto child_elem : actor->GetChildSceneActors())
			{
				UpdateSceneActor(update_info, actor, child_elem.second);
			}
		}
		// destroy at last
		delete old_prefab_info;
	}
	else
	{
		if (actor->GetChildSceneActors().size() > 0)
		{
			for (auto child_elem : actor->GetChildSceneActors())
			{
				UpdateSceneActor(update_info, actor, child_elem.second);
			}
		}
	}
}


void SceneActorFactory::CheckAllSceneActorIntegrityCheck()
{
	if (scene_actor_container.size() > 0)
	{
		for (auto & elem : scene_actor_container)
		{
			std::cout << std::endl;
			std::cout << "checking..." << elem.second->GetSceneActorName()<< std::endl;
			PrefabIntegrityCheck(elem.second);

			if (elem.second->GetChildSceneActors().size() > 0)
			{
				for (auto & child_elem : elem.second->GetChildSceneActors())
				{
					CheckAllSceneActorIntegrityCheck(child_elem.second);
				}
			}
		}
	}
}

void SceneActorFactory::CheckAllSceneActorIntegrityCheck(SceneActor * actor)
{
	PrefabIntegrityCheck(actor);

	if (actor->GetChildSceneActors().size() > 0)
	{
		for (auto & child_elem : actor->GetChildSceneActors())
		{
			CheckAllSceneActorIntegrityCheck(child_elem.second);
		}
	}
}

bool SceneActorFactory::SavePrefabIntegrityCheck(SceneActor * actor)
{
	unsigned prefab_actor_key = GetPrefabKeyByPath(&m_PrefabActorFactory->GetPrefabActorContainer(), actor->GetPrefabActor()->GetPath());
	PrefabActor * prefab = m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_key)->second;

	return m_PrefabActorFactory->PrefabCheck(prefab, actor->GetPrefabActor());
}

void SceneActorFactory::PrefabIntegrityCheck(SceneActor * actor)
{
	unsigned prefab_actor_key = GetPrefabKeyByPath(&m_PrefabActorFactory->GetPrefabActorContainer(), actor->GetPrefabActor()->GetPath());
	PrefabActor * prefab = m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_key)->second;

	bool same_data = m_PrefabActorFactory->PrefabCheck(prefab, actor->GetPrefabActor());
	if (!same_data)
	{
		std::cout << "Process to find any other similar prefab's data in container." << std::endl;
		unsigned prefab_actor_key_with_same_data = m_PrefabActorFactory->FindAnySimilarPrefabData(actor->GetPrefabActor());
		std::string prefab_actor_key_with_same_data_prefab_name = m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_key_with_same_data)->second->GetPrefabActorName();
		auto default_container = m_PrefabActorFactory->GetDefaultPrefabActorContainer();
		
		bool need_create_prefab = false;

		if (default_container.find(prefab_actor_key_with_same_data_prefab_name) != default_container.end())
		{
			need_create_prefab = true;
		}
	
		if (need_create_prefab)
		{
			std::cout << "Can't found any matching prefab's data for " << actor->GetSceneActorName() << " ,it will automative create new prefab's data." << std::endl;
			PrefabActor * new_prefab = new PrefabActor(*actor->GetPrefabActor());
			
			std::string tmp_path = "Resource/Json/PrefabActor/" + new_prefab->GetPrefabActorName() + ".json";
			new_prefab->SetPath(tmp_path);
			new_prefab->SetPrefabActorName(&m_PrefabActorFactory->GetPrefabActorContainer(), actor->GetSceneActorName());

			unsigned prefab_key = 1;

			while (m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_key) != m_PrefabActorFactory->GetPrefabActorContainer().end())
			{
				++prefab_key;
			}
			m_PrefabActorFactory->GetPrefabActorContainer().emplace(prefab_key, new_prefab);
			m_PrefabActorFactory->SaveAllPrefabActor();

			actor->GetPrefabActor()->SetPath(m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_key)->second->GetPath());
			actor->GetPrefabActor()->SetPrefabActorName(m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_key)->second->GetPrefabActorName());

		}
		else
		{
			std::cout << "scene actor:" << actor->GetSceneActorName() << " will using " << m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_key_with_same_data)->second->GetPrefabActorName() << "'s Data" << std::endl;
			actor->GetPrefabActor()->SetPath(m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_key_with_same_data)->second->GetPath());
			actor->GetPrefabActor()->SetPrefabActorName(m_PrefabActorFactory->GetPrefabActorContainer().find(prefab_actor_key_with_same_data)->second->GetPrefabActorName());
		}
	}
}

void SceneActorFactory::SceneActorShiftSelection(SceneActor * last_click)
{
	while (last_click->GetParentSceneActorPointer() != nullptr)
	{
		last_click = last_click->GetParentSceneActorPointer();
	}

	last_click->SetSceneActorSelection(true);

	bool toggle = true;

	SceneActor* top = nullptr;
	SceneActor* bottom = nullptr;

	for (auto elem : scene_actor_container)
	{
		if (elem.second == last_click)
		{
			toggle = false;
			continue;
		}

		if (toggle)
		{
			if (elem.second->GetSceneActorSelection())
			{
				top = elem.second;
			}
		}
		else
		{
			if (elem.second->GetSceneActorSelection())
			{
				bottom = elem.second;
				break;
			}
		}
	}

	if (top != nullptr)
	{
		bool begin_set_selection = false;
		for (auto elem : scene_actor_container)
		{
			if (elem.second == top)
			{
				begin_set_selection = true;
			}

			if (begin_set_selection)
			{
				elem.second->SetSceneActorSelection(true);
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
		for (auto elem : scene_actor_container)
		{
			if (elem.second == last_click)
			{
				begin_set_selection = true;
			}

			if (begin_set_selection)
			{
				elem.second->SetSceneActorSelection(true);
			}

			if (elem.second == bottom)
			{
				break;
			}
		}
	}
}
unsigned * SceneActorFactory::GetCurrentKeyIDPtr()
{
	return &scene_actor_container_key;
}

void SceneActorFactory::AddToScript(sol::state * luaState)
{

	luaState->new_usertype<SceneActorFactory>("CurretViewCamera",
		// Member functions
		"SwitchCamera", &SceneActorFactory::SetCurrentSceneCamera,
		"GetCurrentSceneCameraPosition", &SceneActorFactory::GetCurrentSceneCameraPosition,
		"ShakeCurrentSceneCamera", &SceneActorFactory::ShakeCurrentSceneCamera

		);
}

SceneActor * SceneActorFactory::GetCurrentSceneCamera()const
{
	return current_scene_camera;
}

void SceneActorFactory::SetCurrentSceneCamera(std::string actor_name, bool lerp)
{
	if (scene_actor_container.find(GetSceneActorKeyByName(&scene_actor_container, actor_name)) != scene_actor_container.end())
	{
		auto new_scene_actor = scene_actor_container.find(GetSceneActorKeyByName(&scene_actor_container, actor_name))->second;
		auto new_comp = new_scene_actor->GetPrefabActor()->GetComponents();
		if (new_comp.find(ComponentType::CameraComponent) != new_comp.end())
		{
			if (current_scene_camera != nullptr && lerp)
			{
				auto old_comp = current_scene_camera->GetPrefabActor()->GetComponents();
				if (old_comp.find(ComponentType::CameraComponent) != old_comp.end())
				{
					CameraComponent* old_cam = dynamic_cast<CameraComponent*>(old_comp.find(ComponentType::CameraComponent)->second);
					CameraComponent* new_cam = dynamic_cast<CameraComponent*>(new_comp.find(ComponentType::CameraComponent)->second);
					new_cam->GetCamera()->CameraSwitch(old_cam->GetCamera()->GetPosition().x, old_cam->GetCamera()->GetPosition().y, old_cam->GetCamera()->GetPosition().z);
					current_scene_camera = new_scene_actor;

				}
				else
				{
					current_scene_camera = new_scene_actor;
				}

			}
			else
			{
				current_scene_camera = new_scene_actor;
			}
		}
	}
}

Vector3D SceneActorFactory::GetCurrentSceneCameraPosition() const
{
	Vector3D pos{ 0,0,0 };
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			pos.x = cam->GetCamera()->GetPosition().x;
			pos.y = cam->GetCamera()->GetPosition().y;
			pos.z = cam->GetCamera()->GetPosition().z;
		}
	}
	return pos;
}

void SceneActorFactory::ShakeCurrentSceneCamera(float amplitude, float frequency, float shake_duration)
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			cam->GetCamera()->SetCameraShake(amplitude, frequency, shake_duration);
		}
	}
}

void SceneActorFactory::SetCurrentSceneCameraViewPosition(float x, float y, float z, bool back_to_object, float timer )
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			cam->GetCamera()->SetViewLocation(x, y, z, back_to_object, timer);
		}
	}
}

void SceneActorFactory::SetCurrentSceneCameraViewObject(std::string actor_name, float z , bool back_to_object, float timer)
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end() && scene_actor_container.find(GetSceneActorKeyByName(&scene_actor_container, actor_name)) != scene_actor_container.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			auto view_scene_actor = scene_actor_container.find(GetSceneActorKeyByName(&scene_actor_container, actor_name))->second;
			cam->GetCamera()->SetViewObject(view_scene_actor, z, back_to_object, timer);
		}
	}
}

void SceneActorFactory::SetCurrentSceneCameraViewBackItself()
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			cam->GetCamera()->ViewBackItself();
		}
	}
}

void SceneActorFactory::SetCurrentSceneCameraLock()
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			cam->GetCamera()->SetViewLock();
		}
	}
}

void SceneActorFactory::SetCurrentSceneCameraUnLock()
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			cam->GetCamera()->SetViewUnLock();
		}
	}
}

void SceneActorFactory::SetCurrentSceneAnchorPoint(unsigned pos, float x, float y )
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			cam->GetCamera()->SetObjectAnchorPoint(pos, x, y);
		}
	}
}

void SceneActorFactory::SetCurrentSceneCameraTempZoom(float zoom, bool back_to_object, float timer )
{
	if (current_scene_camera != nullptr)
	{
		auto comp = current_scene_camera->GetPrefabActor()->GetComponents();
		if (comp.find(ComponentType::CameraComponent) != comp.end())
		{
			CameraComponent* cam = dynamic_cast<CameraComponent*>(comp.find(ComponentType::CameraComponent)->second);
			cam->GetCamera()->SetTempZoom(zoom, back_to_object, timer);
		}
	}
}
