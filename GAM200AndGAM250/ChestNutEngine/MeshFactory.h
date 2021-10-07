/******************************************************************************/
/*!
\file   MeshFactory.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for MeshFactory class

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

#include "Mesh.h"
#include "ResourceFactory.h"


class MeshFactory : public ResourceFactory
{
public:
	MeshFactory();
	~MeshFactory();
	bool LoadFromFile(const char * filename);
	std::unordered_map<std::string, Mesh*> * GetMeshList();
private:
	std::unordered_map<std::string, Mesh*> m_MeshMap;
};
