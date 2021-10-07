#pragma once
/******************************************************************************/
/*!
\file  UndoMove.h

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
#include "TransformComponent.h"

struct UndoMove : public Command
{
	UndoMove(Point2D opos, Point2D newpos, TransformComponent * trans)
		: m_opos{ opos }, m_newpos{ newpos }, m_trans{ trans }
	{}

	void ExecuteRedo() override
	{
		m_trans->SetPosition(m_newpos); //MARK
	}

	void ExecuteUndo() override
	{
		m_trans->SetPosition(m_opos);
	}

	Point2D m_opos; //Original position
	Point2D m_newpos; //new position
	TransformComponent * m_trans; 
};