#include "FontFactory.h"

FontFactory::FontFactory()
{
}

FontFactory::~FontFactory()
{
	for (auto& it : m_FontMap)
	{
		delete it.second;
		it.second = nullptr;
	}
}

void FontFactory::ScanDirectory()
{

}

std::unordered_map<std::string, FontTextureAtlas*> * FontFactory::GetFontList()
{
	return &m_FontMap;
}

bool FontFactory::LoadFromFile(const char * filename)
{
	FontTextureAtlas* fontAtlas;

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
		const rapidjson::Value& fontJson = document["Font"];

		for (rapidjson::SizeType i = 0; i < fontJson.Size(); i++)
		{
			const char * path = fontJson[i]["Path"].GetString();
			const char * hash = fontJson[i]["Hash"].GetString();
			unsigned pixelsize = fontJson[i]["PixelSize"].GetUint();
			fontAtlas = new FontTextureAtlas(path,pixelsize);

			m_FontMap[hash] = fontAtlas;
			fontAtlas = nullptr;
		}
	}
	return true;
}