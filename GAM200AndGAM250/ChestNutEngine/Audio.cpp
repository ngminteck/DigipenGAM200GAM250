/******************************************************************************/
/*!
\file  Audio.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for Audio class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Audio.h"

Audio::Audio()
	:m_loop{false}, m_vol{1.0f}
{
}

Audio::Audio(std::string path, bool loop, float vol)
	:m_path{path}, m_loop{loop}, m_vol{vol}
{
	

}

Audio::~Audio()
{

}

void Audio::SetLoop(bool loop)
{
	m_loop = loop;
}

void Audio::SetVolume(float vol)
{
	//Value checker.
	if (vol > 1.0 || vol < 0.0) { vol = 0.0f; }

	else
		m_vol = vol;
}

void Audio::SetPath(std::string path)
{
	m_path = path;
}
