/******************************************************************************/
/*!
\file   SpriteSheetFactory.cpp

\author Ng Min Teck(50%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\author Wong Zhi Jun(50%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun


\brief
	the file loading for spritesheet and also tool to generate json for texture

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "SpriteSheetFactory.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <sstream>
#include <filesystem>

#include <fstream>
#include <iostream>
#include <stdlib.h>





SpriteSheetFactory::SpriteSheetFactory()
:SpriteSheet_container_key{0}
{

}
SpriteSheetFactory::~SpriteSheetFactory()
{
	for (auto & elem : SpriteSheet_container)
	{
		delete elem.second;
	}
}

void SpriteSheetFactory::ScanSpriteSheetFromDirectory()
{
	std::string filepath = "Resource/Json/SpriteSheet";
	for (auto & p : std::filesystem::directory_iterator{ filepath.c_str() })
	{
		
		std::string slash = "/";
		if (p.path().extension() == ".json")
		{
			
			std::string jsonfile = filepath + slash + p.path().filename().string();

			if (p.path().filename().string().compare("SpriteSheet.json") == 0)
				continue;
			LoadSpriteSheetrFromJsonFile(jsonfile.c_str());
		}

		if (!p.path().has_extension())
		{
			std::string internal_filepath = filepath + slash + p.path().filename().string();
			ScanSpriteSheetFromDirectory(internal_filepath);
		}
	}
}

void  SpriteSheetFactory::ScanSpriteSheetFromDirectory(std::string filepath)
{
	for (auto & p : std::filesystem::directory_iterator{ filepath.c_str() })
	{
		std::string slash = "/";
		if (p.path().extension() == ".json")
		{

			std::string jsonfile = filepath + slash + p.path().filename().string();

			if (p.path().filename().string().compare("SpriteSheet.json") == 0)
				continue;
			LoadSpriteSheetrFromJsonFile(jsonfile.c_str());
		}

		if (!p.path().has_extension())
		{
			std::string internal_filepath = filepath + slash + p.path().filename().string();
			ScanSpriteSheetFromDirectory(internal_filepath);
		}
	}
}

void SpriteSheetFactory::LoadSpriteSheetrFromJsonFile(const char * filepath)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filepath, "r");
	//std::cout << filepath << std::endl;

	if (err == 0)
	{
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		rapidjson::Document document;
		document.ParseStream(is);
		if (document.HasMember("JsonFileType"))
		{
			std::string json_file_type = document["JsonFileType"].GetString();
			if (json_file_type.compare("Sprite") == 0)
			{
				if (document.HasMember("Sprite"))
				{
					std::string filename = filepath;
					std::size_t found = filename.find_last_of("/\\");
					filename = filename.substr((found + 1), filename.find("."));

					if (document["Sprite"].HasMember("DDS_Path") && document["Sprite"].HasMember("Sprite_Name") && document["Sprite"].HasMember("Texture_Container") && document["Sprite"].HasMember("Animated_Texture"))
					{
						Texture2D* tex = new Texture2D(filepath, filename, document["Sprite"]["DDS_Path"].GetString(), document["Sprite"]["Sprite_Name"].GetString(), document["Sprite"]["Animated_Texture"].GetBool());
						const rapidjson::Value& texture_json = document["Sprite"]["Texture_Container"];
						float width = static_cast<float>(tex->GetWidth());
						float height = static_cast<float>(tex->GetHeight());
						for (rapidjson::SizeType i = 0; i < texture_json.Size(); i++)
						{
							std::vector<float> intervals;
							std::string aniName = texture_json[i]["Texture_Name"].GetString();

							float x = texture_json[i]["Frame"]["x"].GetFloat() / width;
							float y = 1 - texture_json[i]["Frame"]["y"].GetFloat() / height; 
							float w = texture_json[i]["Frame"]["w"].GetFloat() / width;
							float h = texture_json[i]["Frame"]["h"].GetFloat() / height;

							int numframes = texture_json[i]["Frame"]["numframes"].GetInt();

							const rapidjson::Value& intervalarray = texture_json[i]["Frame"]["interval"];
							for (rapidjson::SizeType j = 0; j < intervalarray.Size(); ++j)
							{
								intervals.push_back(intervalarray[j].GetFloat());
							}

							Vertex vertex[4] =
							{
								{ glm::vec3{ -0.5,-0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x,y - h } },
								{ glm::vec3{ -0.5, 0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x,y } },
								{ glm::vec3{ 0.5, 0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x + w,y } },
								{ glm::vec3{ 0.5,-0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x + w,y - h } }
							};
							Vector2D min = { x, -(y - h) }, max = { x + w,-y };
							unsigned indices[6] = { 0,1,2,0,2,3 };
							Mesh* mesh = new Mesh{ vertex,4,indices,6 };
							if (w < 0.f)
								w = abs(w);
							Animation* anim = new Animation(mesh, w, h, numframes, intervals,max,min);
							tex->AddAnimation(anim, aniName);
						}
						SpriteSheet_container.emplace(SpriteSheet_container_key, tex);
						++ SpriteSheet_container_key;
					}
				}
			}
		}
		fclose(fp);
	}
	else
	{
		std::cout << "Fail to open " << filepath << " file" << std::endl;
	}
}

void SpriteSheetFactory::ScanNoAnimatedTextureAndOutputToJson()
{
	std::string filepath = "Resource/Art/Scan";
	for (auto & p : std::filesystem::directory_iterator{ filepath.c_str() })
	{
		// mean dir
		if (!p.path().has_extension())
		{
			std::string slash = "/";
			std::string jsonfile = filepath + slash + p.path().filename().string();
			ScanNoAnimatedTextureAndOutputToJson(jsonfile.c_str());

		}
		else
		{
			std::string slash = "/";
			std::string jsonfile = filepath + slash + p.path().filename().string();
			GenerateTexturePNGToJson(jsonfile.c_str());
		}
	}
}

void SpriteSheetFactory::ScanNoAnimatedTextureAndOutputToJson(std::string filepath)
{
	for (auto & p : std::filesystem::directory_iterator{ filepath.c_str() })
	{
		// mean dir
		if (!p.path().has_extension())
		{
			std::string slash = "/";
			std::string jsonfile = filepath + slash + p.path().filename().string();
			ScanNoAnimatedTextureAndOutputToJson(jsonfile.c_str());

		}
		else
		{
			std::string slash = "/";
			std::string jsonfile = filepath + slash + p.path().filename().string();
			GenerateTexturePNGToJson(jsonfile.c_str());
		}
	}
}
void SpriteSheetFactory::GenerateTexturePNGToJson(std::string filepath)
{
	unsigned  width = 0;
	unsigned  height = 0;
	bool output_json = false;
	std::ifstream in(filepath, std::ios_base::binary | std::ios_base::in);
	if (in.is_open())
	{
		in.seekg(16, std::ios_base::cur);
		unsigned  w0;

		in.read((char *)&w0, 4);
		width = ((w0 >> 24) & 0xff) |
			((w0 << 8) & 0xff0000) |
			((w0 >> 8) & 0xff00) |
			((w0 << 24) & 0xff000000);

		in.read((char *)&w0, 4);
		height = ((w0 >> 24) & 0xff) |
			((w0 << 8) & 0xff0000) |
			((w0 >> 8) & 0xff00) |
			((w0 << 24) & 0xff000000);

		output_json = true;
		in.close();
	}
	if (output_json)
	{
		std::string original_filepath = filepath;
		std::size_t found = original_filepath.find_last_of(".");
		std::size_t foundslash = original_filepath.find_last_of("/\\");

		std::string save_json_path = original_filepath.substr(0, found) + ".json";
		
		std::string sprite_name = original_filepath.substr(foundslash + 1, original_filepath.size());
		sprite_name = sprite_name.substr(0,sprite_name.find("."));

		std::string dds_filepath = ""; 
		dds_filepath = "Resource/Art/DDS/UI/" + sprite_name + ".dds";
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

		std::stringstream file_name;
		file_name.str(std::string());
		file_name << save_json_path;

		writer.SetMaxDecimalPlaces(1);

		writer.StartObject();

		writer.Key("JsonFileType");
		writer.String("Sprite");

		writer.Key("Sprite");
		writer.StartObject();
		writer.Key("DDS_Path");
		writer.String(dds_filepath.c_str());
		writer.Key("Sprite_Name");
		writer.String(sprite_name.c_str());
		writer.Key("Animated_Texture");
		writer.Bool(false);

		writer.Key("Texture_Container");

		writer.StartArray();

		writer.StartObject();

		writer.Key("Texture_Name");
		writer.String(sprite_name.c_str());

		writer.Key("Frame");

		writer.StartObject();

		writer.Key("x");
		writer.Int(0);
		writer.Key("y");
		writer.Int(0);
		writer.Key("w");
		writer.Uint(width);
		writer.Key("h");
		writer.Uint(height);
		writer.Key("numframes");
		writer.Int(1);
		writer.Key("interval");
		writer.StartArray();

		writer.Double(0);

		writer.EndArray();
		writer.EndObject();

		writer.EndObject();

		writer.EndArray();

		writer.EndObject();

		writer.EndObject();

		std::ofstream of;

		of.open(file_name.str().c_str());
		of << s.GetString();
		if (!of.good())
			throw std::runtime_error("Failed to save the Level!");
	}
	else
	{
		std::cout << "Unable to read: " << filepath << std::endl;
	}
}

unsigned GetSpriteKeyBySpriteName(std::unordered_map<unsigned, Texture2D *> * container, std::string search)
{
	std::string search_path = search;
	std::transform(search_path.begin(), search_path.end(), search_path.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = container->begin(); it != container->end(); ++it)
	{
		std::string container_name_id = it->second->GetTextureName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_path.compare(container_name_id) == 0)
			return it->first;
	}
	// 0 is default prefab from the prefab actor factory
	return 0;
}

