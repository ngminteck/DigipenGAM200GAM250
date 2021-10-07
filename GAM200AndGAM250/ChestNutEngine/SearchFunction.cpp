/******************************************************************************/
/*!
\file   SearchFunction.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief


All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "SearchFunction.h"

SearchFunction::SearchFunction()
{

}

SearchFunction::~SearchFunction()
{

}



std::vector<SearchInfo> * SearchFunction::GameObjectInstanceSearch(const std::unordered_map<unsigned, GameObject *> * GameObjectInstance, std::string search)
{
	search_vector.clear();
	std::string search_id = search;
	std::transform(search_id.begin(), search_id.end(), search_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

	for (auto it = GameObjectInstance->begin(); it != GameObjectInstance->end(); ++it)
	{
		std::string container_name_id = it->second->GetIDName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
		if (search_id.compare(container_name_id) == 0)
		{
			SearchInfo searchinfo;
			searchinfo.container_type = "GameObjectInstance";
			searchinfo.container_key_index = it->first;
			search_vector.push_back(searchinfo);

		}
	}

	for (auto it = GameObjectInstance->begin(); it != GameObjectInstance->end(); ++it)
	{
		std::string container_name_id = it->second->GetIDName();
		std::transform(container_name_id.begin(), container_name_id.end(), container_name_id.begin(), [](char c) {return static_cast<char>(::tolower(c)); });

		const char *ptr = strstr(container_name_id.c_str(), search_id.c_str());

		if (ptr != NULL)
		{
			SearchInfo searchinfo;
			searchinfo.container_type = "GameObjectInstance";
			searchinfo.container_key_index = it->first;
			bool not_in_list = true;
			for (auto searchit = search_vector.begin(); searchit != search_vector.end(); ++searchit)
			{
				if (searchit->container_key_index == searchinfo.container_key_index)
					not_in_list = false;
			}
			if(not_in_list)
				search_vector.push_back(searchinfo);
		}
	}
	return &search_vector;
}

