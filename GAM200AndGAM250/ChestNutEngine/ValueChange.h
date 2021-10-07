#pragma once
/******************************************************************************/
/*!
\file  ValueChange.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for Inspector class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Command.h"

template<typename T>
struct ValueChange : public Command
{
	ValueChange(T * var, T ori, T n)
	:variable{ var }, original_value{ ori }, new_value{n} {}

	void ExecuteRedo() override { *variable = new_value;}

	void ExecuteUndo() override { *variable = original_value;}

	T * variable;
	T original_value;
	T new_value;
};