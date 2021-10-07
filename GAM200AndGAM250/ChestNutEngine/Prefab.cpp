#include "Prefab.h"

Prefab::Prefab()
{
}

Prefab::~Prefab()
{
	EmptyComponents();
}

void Prefab::EmptyComponents()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}

	m_components.clear();
}