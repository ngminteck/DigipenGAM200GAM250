/******************************************************************************/
/*!
\file   SearchFunction.h

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	For searching asset

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include <unordered_map>
#include "GameObject.h"


struct SearchInfo
{
	std::string container_type;
	size_t container_key_index;
};

class SearchFunction
{
public:
	SearchFunction();
	~SearchFunction();
	

	std::vector<SearchInfo> * GameObjectInstanceSearch(const std::unordered_map<unsigned, GameObject *> * prefab, std::string search);

	

private:
	std::vector<SearchInfo> search_vector;
	

};