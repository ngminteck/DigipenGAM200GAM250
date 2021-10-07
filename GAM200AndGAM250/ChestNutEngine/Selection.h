/******************************************************************************/
/*!
\file  Selection.h

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
#pragma once

class Selection
{
public:
	Selection();
	~Selection();
	bool GetSelection() const;
	void SetSelection(bool value);

private:
	bool select;
};