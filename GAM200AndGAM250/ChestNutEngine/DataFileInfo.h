/******************************************************************************/
/*!
\file   DataFileInfo.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the file info , filename and file path

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include <unordered_map>
#include <string>

class DataFileInfo
{
public:
	DataFileInfo();
	DataFileInfo(std::string path, std::string filename );
	DataFileInfo(const DataFileInfo & rhs);
	virtual ~DataFileInfo();
	std::string & GetFilename();
	void SetFileName(std::string filename);
	std::string & GetPath();
	void SetPath(std::string filename);

private:
	std::unordered_map<std::string, std::string> resource_info;
};