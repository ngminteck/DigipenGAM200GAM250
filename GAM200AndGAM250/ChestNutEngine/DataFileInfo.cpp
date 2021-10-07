/******************************************************************************/
/*!
\file   DataFileInfo.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the file info , filename and file path

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "DataFileInfo.h"

DataFileInfo::DataFileInfo()
{
	std::string filename_key = "filename";
	std::string filename_value = "";
	std::string path_key = "path";
	std::string path_value = "";
	resource_info.emplace(filename_key, filename_value);
	resource_info.emplace(path_key, path_value);
}

DataFileInfo::DataFileInfo(std::string path, std::string filename)
{
	std::string filename_key = "filename";
	std::string filename_value = filename;
	std::string path_key = "path";
	std::string path_value = path;
	resource_info.emplace(filename_key, filename_value);
	resource_info.emplace(path_key, path_value);
}

DataFileInfo::~DataFileInfo()
{
	resource_info.clear();
}

DataFileInfo::DataFileInfo(const DataFileInfo & rhs)
{
	std::string filename_key = "filename";
	std::string filename_value = rhs.resource_info.find(filename_key)->second;
	std::string path_key = "path";
	std::string path_value = rhs.resource_info.find(path_key)->second;
	resource_info.emplace(filename_key, filename_value);
	resource_info.emplace(path_key, path_value);
}

std::string & DataFileInfo::GetFilename()
{
	return resource_info.find("filename")->second;
}

void DataFileInfo::SetFileName(std::string filename)
{
	resource_info.find("filename")->second = filename;
}

std::string & DataFileInfo::GetPath()
{
	return resource_info.find("path")->second;
}

void DataFileInfo::SetPath(std::string path)
{
	resource_info.find("path")->second = path;
}
