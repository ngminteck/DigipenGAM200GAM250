/******************************************************************************/
/*!
\file   ShaderFactory.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for ShaderFactory class

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

#include "Shader.h"
#include "ResourceFactory.h"


class ShaderFactory : public ResourceFactory
{
public:
	ShaderFactory();
	~ShaderFactory();
	bool LoadFromFile(const char * filename);
	std::unordered_map<std::string, Shader*> * GetShaderList();
private:
	std::unordered_map<std::string, Shader*> m_ShaderList;
};
