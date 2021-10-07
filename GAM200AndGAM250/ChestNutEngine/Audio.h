#pragma once
/******************************************************************************/
/*!
\file  Audio.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for Audio class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
#include <string>
struct Audio
{
	Audio();
	Audio(std::string, bool = false, float = 1.0f);
	~Audio();
	
	
	void SetLoop(bool);
	void SetVolume(float);
	void SetPath(std::string);
	bool m_loop;
	float m_vol;
	float m_pause = 0.0f; //Whats the volume if it is paused.
	std::string m_path;
};