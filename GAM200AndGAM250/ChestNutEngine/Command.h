#pragma once
/******************************************************************************/
/*!
\file  Command.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for Inspector class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
struct Command
{
	virtual void ExecuteRedo() = 0;
	virtual void ExecuteUndo() = 0;
};


