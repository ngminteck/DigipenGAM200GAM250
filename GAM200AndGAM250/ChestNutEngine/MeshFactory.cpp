/******************************************************************************/
/*!
\file   MeshFactory.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for MeshFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "MeshFactory.h"

MeshFactory::MeshFactory()
{

}
MeshFactory::~MeshFactory()
{
	for (auto& it : m_MeshMap)
	{
		delete it.second;
		it.second = nullptr;
	}
}

bool MeshFactory::LoadFromFile(const char * filename)
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
		const rapidjson::Value& meshJson = document["Mesh"]; 

		for (rapidjson::SizeType i = 0; i < meshJson.Size(); i++)
		{
			const rapidjson::Value& vertexJson = meshJson[i]["Vertex"];
			Vertex * vertexArray = new Vertex[vertexJson.Size()];
			for (rapidjson::SizeType j = 0; j < vertexJson.Size(); j++)
			{
				//if (j != vertex[j]["VertexIndex"].GetInt())
				//	std::cout << "vertex order wrong!" << std::endl;

				vertexArray[j] =  { 
									glm::vec3(
												vertexJson[j]["position"]["position1"].GetFloat(),
												vertexJson[j]["position"]["position2"].GetFloat(),
												vertexJson[j]["position"]["position3"].GetFloat()
											), 
									glm::vec3(
												vertexJson[j]["color"]["color1"].GetFloat(),
												vertexJson[j]["color"]["color2"].GetFloat(),
												vertexJson[j]["color"]["color3"].GetFloat()
											),
									glm::vec2(
												vertexJson[j]["texcoord"]["texcoord1"].GetFloat(),
												vertexJson[j]["texcoord"]["texcoord2"].GetFloat()
									) 
				};
			}
			
			const rapidjson::Value& indicesJson = meshJson[i]["indices"];
			unsigned * indexArray = new unsigned[indicesJson.Size()];
			for (rapidjson::SizeType k = 0; k < indicesJson.Size(); k++)
			{
				//if (k != indices[k]["IndicesIndex"].GetInt())
				//	std::cout << "indices order wrong!" << std::endl;
				indexArray[k] = indicesJson[k]["indicesVertex"].GetUint();
				
			}
			
			Mesh * mesh = new Mesh{ vertexArray , vertexJson.Size(),indexArray, indicesJson.Size() }; 
			std::string name = meshJson[i]["MeshName"].GetString(); // Get the name of the string

			m_MeshMap.insert({ name, mesh });

			delete []indexArray;
			delete []vertexArray;
		}
	
		fclose(fp);
		return true;
	}
	else
	{
		std::cout << "Fail to open " << filename << " file" << std::endl;
		return false;
	}
}
std::unordered_map<std::string, Mesh*> * MeshFactory::GetMeshList()
{
	return &m_MeshMap;
}