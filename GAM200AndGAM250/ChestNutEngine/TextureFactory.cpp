/******************************************************************************/
/*!
\file   TextureFactory.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for TextureFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "TextureFactory.h"

TextureFactory::TextureFactory()
{

}
TextureFactory::~TextureFactory()
{
	for (auto& it : m_TextureMap) 
	{
		delete it.second;
		it.second = nullptr;
	}
}

void TextureFactory::ScanDirectory()
{
	std::string filepath = "NewResource/Texture";
	for (auto & p : std::experimental::filesystem::v1::directory_iterator(filepath.c_str()))
	{
		if (p.path().extension() == ".png")
		{
			std::string slash = "/";
			std::string texturefile = filepath + slash + p.path().filename().string();
			LoadFromFile(texturefile.c_str());
		}
	}

	 filepath = "NewResource/Sprite";

	 for (auto & p : std::experimental::filesystem::v1::directory_iterator(filepath.c_str()))
	 {
		 if (p.path().extension() == ".png")
		 {
			 std::string slash = "/";
			 std::string texturefile = filepath + slash + p.path().filename().string();
			 LoadFromFile(texturefile.c_str());
		 }
	 }
}

bool TextureFactory::LoadFromFile(const char * filename)
{
	Texture2D* tex;

	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filename, "r");
	if (err == 0)
	{
		std::cout << "Loading content of " << filename << " file" << std::endl;

		char readBuffer[65536]; // assign the buffersize
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); // rapidjson file reader stream
		rapidjson::Document document; // rapidjson document for using rpaidjson document format
		document.ParseStream(is); // pass in the file stream into the document
		const rapidjson::Value& textureJson = document["Texture"];

		for (rapidjson::SizeType i = 0; i < textureJson.Size(); i++)
		{
			const char * DDSPath = textureJson[i]["DDSPath"].GetString();
			//const char * hash	= textureJson[i]["Hash"].GetString();
			//const char * anidatapath = textureJson[i]["AnimationData"].GetString();


			if (m_TextureMap.find(DDSPath) == m_TextureMap.end())
			{
				// Create the Texture
				// using path

//				tex = new Texture2D(DDSPath);
//				m_TextureMap[DDSPath] = tex;
			//	LoadAnimationFile(anidatapath, tex);
			}



			tex = nullptr;
		}
	}
	fclose(fp);
	return true;
}

bool TextureFactory::LoadAnimationFile(const char * filename, Texture2D* tex)
{
	Animation* animation;

	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filename, "r");
	if (err == 0)
	{
		std::cout << "Loading content of " << filename << " file" << std::endl;

		char readBuffer[65536]; // assign the buffersize
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); // rapidjson file reader stream
		rapidjson::Document document; // rapidjson document for using rpaidjson document format
		document.ParseStream(is); // pass in the file stream into the document	
		const rapidjson::Value& animationJson = document["Animation"];
		float width = static_cast<float>(tex->GetWidth());
		float height = static_cast<float>(tex->GetHeight());
		float x, y, w, h, interval;
		int numframes;
		for (rapidjson::SizeType i = 0; i < animationJson.Size(); i++)
		{
			std::string aniName = animationJson[i]["Name"].GetString();
			std::string objName = animationJson[i]["ObjectName"].GetString();
			x			= animationJson[i]["Frame"]["x"].GetFloat() / width;
			y		    = 1-animationJson[i]["Frame"]["y"].GetFloat() / height;
			w			= animationJson[i]["Frame"]["w"].GetFloat() / width;
			h			= animationJson[i]["Frame"]["h"].GetFloat() / height;
			interval	= animationJson[i]["Frame"]["interval"].GetFloat();
			numframes	= animationJson[i]["Frame"]["numframes"].GetInt();
			Vertex vertex[4] = 
			{
				{ glm::vec3{ -0.5,-0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x,y-h } },
				{ glm::vec3{ -0.5, 0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x,y } },
				{ glm::vec3{  0.5, 0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x+w,y } },
				{ glm::vec3{  0.5,-0.5,0.0f },glm::vec3{ 0.f,0.f,0.f },glm::vec2{ x+w,y-h } }
			};
			unsigned indices[6] = { 0,1,2,0,2,3 };
			Mesh* mesh= new Mesh{ vertex,4,indices,6 };
			if (w < 0.f)
				w*=-1.f;
			animation = new Animation(mesh,w,h,numframes, interval);
			tex->AddAnimation(animation, aniName );
			animation = nullptr;
		}
	}
	return true;
}


	std::unordered_map<std::string, Texture2D*> * TextureFactory::GetTextureList()
{
	return &m_TextureMap;
}