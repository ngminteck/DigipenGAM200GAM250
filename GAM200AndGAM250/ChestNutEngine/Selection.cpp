/******************************************************************************/
/*!
\file  Selection.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
This file contains the base selection behaviour for all asset, that use in 
editor

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Selection.h"

Selection::Selection()
:select{false}
{

}

Selection::~Selection()
{

}

bool Selection::GetSelection() const
{
	return select;
}

void Selection::SetSelection(bool value)
{
	select = value;
}