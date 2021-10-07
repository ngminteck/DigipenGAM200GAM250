/******************************************************************************/
/*!
\file   ResourceFactory.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for ResourceFactory class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once


#include <filesystem>

class ResourceFactory
{
public:
	ResourceFactory() {}
	virtual ~ResourceFactory() {}
	virtual bool LoadFromFile(const char * filename) = 0;
};	