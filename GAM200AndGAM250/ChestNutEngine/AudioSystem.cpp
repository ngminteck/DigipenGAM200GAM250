/******************************************************************************/
/*!
\file  AudioSystem.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for AudioSystem class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "AudioSystem.h"
#define NOADUIO
AudioSystem::AudioSystem(EventHandler * ev, bool & pause) 
:m_ev{ ev }, m_pause{ pause }, m_master_volume{ 1.0f }
{
	//Will update the state of the player
	m_ev->SubscribeEvent(this, &AudioSystem::RestartAudio);
	//m_ev->SubscribeEvent(this, &AudioSystem::PauseAudio);
	m_ev->SubscribeEvent(this, &AudioSystem::RunSingleAudio);
	m_ev->SubscribeEvent(this, &AudioSystem::ReInitialize);
	sgpImplementation = new Implementation{};
}

void AudioSystem::Init(std::map<unsigned, SceneActor *> & obj)
{
	m_objs = &obj;
}

void AudioSystem::Update()
{
	float pause = (m_pause) ? 0.1f : 1.0f;
	UnpauseAllChannels();
	if (m_objs->size())
	{
		for (auto & obj : *m_objs)
		{
			auto & channelMap = sgpImplementation->mChannels;
			AudioComponent	  * audio = nullptr;
			MovementComponent * move = nullptr;
			TransformComponent * trans = nullptr;
			std::string name = obj.second->GetSceneActorName();
			float distance = 0.0f;
			float proximity = 1.0f;

			auto & comps = obj.second->GetPrefabActor()->GetComponents();
			if (comps.find(ComponentType::AudioComponent) != comps.end())
				audio = dynamic_cast<AudioComponent *>(comps[ComponentType::AudioComponent]);
			if (comps.find(ComponentType::MovementComponent) != comps.end())
				move = dynamic_cast<MovementComponent *>(comps[ComponentType::MovementComponent]);
			if (comps.find(ComponentType::TransformComponent) != comps.end())
				trans = dynamic_cast<TransformComponent *>(comps[ComponentType::TransformComponent]);


			if (audio)
			{
				int & channelID = audio->channelID;
				auto & audioMap = audio->audioMap;
				//Normally for background
				if (trans == nullptr && move == nullptr)
				{
					if (channelID == -1)
					{
						channelID = Play(audio->audioMap["IDLE"].m_path, { 0,0,0 }, VolumeToDB(audioMap["IDLE"].m_vol), audioMap["IDLE"].m_loop);
					}

					//Update the volume.
					channelMap[channelID]->setVolume(pause * proximity * m_master_volume * audioMap["IDLE"].m_vol);
				}


				//IF NO MOVEMENT COMPONENT
				if (audio && trans && !move)
				{
					if (m_player)
					{
						//Calculate the distance between the object and the player.
						distance = Vector2DLengthSquared(trans->GetPosition() - m_player->GetPosition());

						if (distance > 100.0f)
							continue;

					}

					proximity -= distance / 100.0f;
					if (channelID == -1)
					{
						channelID = Play(audio->audioMap["IDLE"].m_path, { 0,0,0 }, VolumeToDB(audioMap["IDLE"].m_vol), audioMap["IDLE"].m_loop);
					}

					//Update the volume.
					channelMap[channelID]->setVolume(pause * proximity * m_master_volume * audioMap["IDLE"].m_vol);
				}

				//IF ITS ENEMY
				if (audio && trans && move)
				{
					if (m_player)
					{
						//Calculate the distance between the object and the player.
						distance = Vector2DLengthSquared(trans->GetPosition() - m_player->GetPosition());

						if (distance > 150.0f)
							continue;

					}

					proximity -= distance / 150.0f;


					std::string state = move->GetStateLogic();
					if (move->GetStateLogic() != move->GetPrevStateLogic())
					{
						StopChannel(channelID);
						channelID = Play(audioMap[state].m_path, { 0,0,0 }, VolumeToDB(audioMap[state].m_vol), audioMap[state].m_loop);
					}

					else
					{
						bool play = false;
						//Check if the clip has ended.
						channelMap[channelID]->isPlaying(&play);

						//If it has stopped playing, play it again.
						if (play == false && audioMap[state].m_loop)
							channelID = Play(audioMap[state].m_path, { 0,0,0 }, VolumeToDB(audioMap[state].m_vol), audioMap[state].m_loop);
					}

					//Update the volume.
					channelMap[channelID]->setVolume(pause * proximity * m_master_volume * audioMap["IDLE"].m_vol);
				}
			}
		}
	}

	sgpImplementation->Update();

	//else
		//PauseAllChannels();
		
}


void AudioSystem::RunSingleAudio(PlayAudio * audio)
{
	int channel = Play(audio->m_audio.m_path, Vector3D{ 0,0,0 }, audio->m_audio.m_vol);
	sgpImplementation->mChannels[channel]->setVolume(audio->m_audio.m_vol);
}



void AudioSystem::PauseAudio(PauseGame * ev)
{
	UNREFERENCED_PARAMETER(ev);
	m_pause = !m_pause;
	if (m_pause)
		PauseAllChannels();
	else
		UnpauseAllChannels();
}



void AudioSystem::RestartAudio(LoadLevel * ev)
{
	UNREFERENCED_PARAMETER(ev);
	StopAllChannels();

	if (m_pause)
		PauseAllChannels();
	else
		UnpauseAllChannels();
}



void AudioSystem::PauseAllChannels()
{
	for (auto & channel : sgpImplementation->mChannels)
		channel.second->setPaused(true);
}


void AudioSystem::UnpauseAllChannels()
{
	for (auto & channel : sgpImplementation->mChannels)
		channel.second->setPaused(false);
}

void AudioSystem::StopChannel(int id)
{
	sgpImplementation->mChannels[id]->stop();
}

void AudioSystem::StopAllChannels()
{
	for (auto & channel : sgpImplementation->mChannels)
		channel.second->stop();
}

void AudioSystem::Shutdown()
{
	delete sgpImplementation;
}

AudioSystem::~AudioSystem()
{
	Shutdown();
}

int AudioSystem::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		//std::cout << "FMOD ERROR " << result << std::endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

void AudioSystem::LoadSound(const std::string & strSoundName, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	AudioSystem::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) 
		sgpImplementation->mSounds[strSoundName] = pSound;

}

void AudioSystem::UnloadSound(const std::string & strSoundName)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;

	AudioSystem::ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}

int AudioSystem::Play(const std::string & strSoundName, const Vector3D & vPos, float fVolumedB, bool loop)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		LoadSound(strSoundName, true, loop, false);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return nChannelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	AudioSystem::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) 
		{
			FMOD_VECTOR position = VectorToFMOD(vPos);
			AudioSystem::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}

		AudioSystem::ErrorCheck(pChannel->setVolume(DBToVolume(fVolumedB)));
		AudioSystem::ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}

void AudioSystem::SetChannel3DPosition(int nChannelId, const Vector3D & vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFMOD(vPosition);
	AudioSystem::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void AudioSystem::SetChannelVolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	AudioSystem::ErrorCheck(tFoundIt->second->setVolume(DBToVolume(fVolumedB)));
}

float AudioSystem::DBToVolume(float db)
{
	return powf(10.0f, 0.05f * db);
}

float AudioSystem::VolumeToDB(float volume)
{
	return 20.0f * log10f(volume);
}

void AudioSystem::ReInitialize(LoadLevel *)
{
	for (auto & obj : *m_objs)
	{
		if (obj.second->GetSceneActorName() == "Surtur")
		{
			m_player = dynamic_cast<TransformComponent *>(obj.second->GetComponent(ComponentType::TransformComponent));
			if (m_player)
			{
				std::cout << "Surtur's camera found." << std::endl;
				break;
			}
		}
	}
}

FMOD_VECTOR AudioSystem::VectorToFMOD(const Vector3D & vPosition)
{
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

AudioSystem::Implementation::Implementation()
{
	mpSystem = NULL;
	AudioSystem::ErrorCheck(FMOD::System_Create(&mpSystem));
	mpSystem->init(512, FMOD_INIT_NORMAL, 0);
}

AudioSystem::Implementation::~Implementation() {

}

void AudioSystem::Implementation::RemoveChannel(int channelID)
{
	auto it = mChannels.find(channelID);
	if (it != mChannels.end())
		mChannels.erase(it);
}

void AudioSystem::Implementation::Update()
{
	mpSystem->update();
	std::vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying); //Check if the sounds has finished playing
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
}

void AudioSystem::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<AudioSystem>("audio",
		"Play", &AudioSystem::Play,
		"StopAllChannel", &AudioSystem::StopAllChannels,
		"m_master_volume", &AudioSystem::m_master_volume
		);
}
