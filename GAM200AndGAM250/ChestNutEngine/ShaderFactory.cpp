#include "ShaderFactory.h"

ShaderFactory::ShaderFactory()
{

}

ShaderFactory::~ShaderFactory()
{
	for (auto& it : m_ShaderList)
	{
		delete it.second;
		it.second = nullptr;
	}
}

bool ShaderFactory::LoadFromFile(const char * filename)
{
	Shader* shader;

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
			const char * vertexShader	= textureJson[i]["Vertex"].GetString();
			const char * fragShader		= textureJson[i]["Fragment"].GetString();
			const char * shaderHash		= textureJson[i]["Hash"].GetString();
			shader = new Shader(vertexShader, fragShader);

			m_ShaderList[shaderHash] = shader;
			shader = nullptr;
		}
	}
	fclose(fp);
	std::cout << "Load Complete" << std::endl;
	return true;
}

std::unordered_map<std::string, Shader*> * ShaderFactory::GetShaderList()
{
	return &m_ShaderList;
}