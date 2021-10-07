#pragma once
#include "Animation.h"
#include "ResourceFactory.h"

class AnimationFactory : public ResourceFactory
{
public:
	AnimationFactory();
	~AnimationFactory();
	bool LoadFromFile(const char * filename);
	std::unordered_map<std::string, AnimationMap*> * GetAnimationList();
private:
	std::unordered_map<std::string, AnimationMap*> m_AnimationMap;
};