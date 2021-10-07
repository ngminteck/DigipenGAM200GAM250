#pragma once
#include "Component.h"
struct VolumeComponent : Component
{
	VolumeComponent();
	~VolumeComponent();

	int m_id;
	bool m_selected;
};