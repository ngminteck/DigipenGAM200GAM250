/******************************************************************************/
/*!
\file   TextureFactory.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for TextureFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"

#include "Texture2D.h"
#include "ResourceFactory.h"
class TextureFactory : public ResourceFactory
{
public:
	TextureFactory();
	~TextureFactory();
	void ScanDirectory();
	bool LoadFromFile(const char * filename);
	bool LoadAnimationFile(const char * filename, Texture2D* tex);
	std::unordered_map<std::string, Texture2D*> * GetTextureList();
private:
	std::unordered_map<std::string, Texture2D*> m_TextureMap;
};