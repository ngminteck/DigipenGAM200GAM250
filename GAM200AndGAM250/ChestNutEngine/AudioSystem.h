#pragma once
/******************************************************************************/
/*!
\file  AudioSystem.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for AudioSystem class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#ifndef _AUDIO_SYSTEM_H_
#define _AUDIO_SYSTEM_H_

#include "../Dep/FMOD/api/lowlevel/inc/fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include "Vector3D.h"
#include "System.h"
#include "EventHandler.h"
#include "AudioComponent.h"
#include "PauseGame.h"
#include "LoadLevel.h"
#include "SceneActor.h"
#include "PlayAudio.h"
#include "ChangeAudio.h"
#include "LoadGameState.h"
#include <sol.hpp>
#endif

class AudioSystem : System 
{
public:
	
	AudioSystem(EventHandler *, bool &);
	~AudioSystem();
	void Init(std::map<unsigned, SceneActor*> &);
	void Update();
	void Shutdown();
	static int ErrorCheck(FMOD_RESULT result);
	void LoadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
	void UnloadSound(const std::string& strSoundName);

	int Play(const std::string& strSoundName, const Vector3D& vPos = Vector3D{ 0, 0, 0 }, float fVolumedB = 0.0f, bool loop = false);

	void RunSingleAudio(PlayAudio * );
	void PauseAudio(PauseGame *);
	void RestartAudio(LoadLevel *);
	void StopChannel(int nChannelId);
	void PauseAllChannels();
	void StopAllChannels();
	void UnpauseAllChannels();

	void SetChannel3DPosition(int nChannelId, const Vector3D& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	float DBToVolume(float db);
	float VolumeToDB(float volume);
	
	void ReInitialize(LoadLevel *);
	FMOD_VECTOR VectorToFMOD(const Vector3D& vPosition);
	struct Implementation 
	{
		Implementation();
		~Implementation();

		void Update();
		FMOD::System* mpSystem;

		int mnNextChannelId = 0;
		void RemoveChannel(int);
		typedef std::map<std::string, FMOD::Sound*> SoundMap;
		typedef std::map<int, FMOD::Channel*> ChannelMap;
		SoundMap mSounds;
		ChannelMap mChannels;
	};

	Implementation* sgpImplementation = nullptr;
	std::map<unsigned, SceneActor*> * m_objs;

	EventHandler * m_ev;
	bool & m_pause;
	float m_master_volume;
	TransformComponent * m_player = nullptr;
	static void AddToScript(sol::state * luaState);
};

