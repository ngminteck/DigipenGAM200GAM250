/******************************************************************************/
/*!
\file   Utils.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains macros, functions and structs for Utils

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once

#define SafeDelete(x) if(x){delete x; x = nullptr;}			// Safely delete a Pointer
#define SafeArrayDelete(x) if(x){delete[]x; x=nullptr;}		// Safely delete an Array of Pointers