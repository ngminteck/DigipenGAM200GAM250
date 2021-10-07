#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"

#include "Font.h"
#include "ResourceFactory.h"

class FontFactory : public ResourceFactory
{
public:
	FontFactory();
	~FontFactory();

	void ScanDirectory();
	bool LoadFromFile(const char * filename);
	std::unordered_map<std::string, FontTextureAtlas*> * GetFontList();
private:

	std::unordered_map<std::string, FontTextureAtlas*> m_FontMap;
};