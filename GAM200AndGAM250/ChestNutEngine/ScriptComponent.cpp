#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(sol::state *lstate)
	:Component{ComponentType::ScriptComponent}, m_luaState{ lstate }
{
}
ScriptComponent::ScriptComponent(const ScriptComponent & rhs)
	: Component{ ComponentType::ScriptComponent }, m_luaState{ rhs.m_luaState }
{
	for (auto& elem: rhs.m_scriptList)
	{
		// Deep copy
		AddScript(elem.first, elem.second->GetPath());
	}
}
ScriptComponent::~ScriptComponent()
{
	for (auto& elem : m_scriptList)
	{
		delete elem.second;
	}
}

Script* ScriptComponent::GetScript(std::string state)
{
	if (m_scriptList.find(state) == m_scriptList.end())
	{
		return nullptr;
	}
	return m_scriptList.find(state)->second;
}
sol::state* ScriptComponent::GetLuaState()
{
	return m_luaState;
}
void ScriptComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("ScriptComponent");
	if (m_scriptList.size() > 0)
	{
		writer.Key("Script");
		writer.StartArray();
		for (auto & elem : m_scriptList)
		{
			writer.StartObject();
			writer.Key("Path");
			writer.String(elem.second->GetPath().c_str());
			writer.Key("State");
			writer.String(elem.first.c_str());
			writer.EndObject();
		}
		writer.EndArray();
	}
}
Component * ScriptComponent::Clone() const
{
	return new ScriptComponent(*this);
}

void ScriptComponent::InitScripts(SceneActor* sceneactor,SceneActor* player)
{
	for (auto& elem : m_scriptList)
	{
		Script* script = elem.second;
		sol::function Start;
		m_luaState->script_file(script->GetPath(), script->GetEnvironment());
		Start = script->GetEnvironment()["Start"];
		if (sceneactor == player)
		{
			auto result = Start(sceneactor);
			if (!Start.valid()) {
				// An error has occured
				sol::error err = result;
				std::string what = err.what();

				std::cout << what << std::endl;
			}
		}
		else
		{
			auto result = Start(sceneactor, player);
			if (!Start.valid()) {
				// An error has occured
				sol::error err = result;
				std::string what = err.what();

				std::cout << what << std::endl;
			}
		}
	}
}
void ScriptComponent::AddScript(std::string state, std::string scriptpath)
{
	m_scriptList.emplace(state,new Script{scriptpath,m_luaState });
	//std::cout << "script component "<< sizeof(m_scriptList)<< m_scriptList.size() << "\n";
}

void ScriptComponent::RemoveScript(std::string state)
{
	if (m_scriptList.find(state) == m_scriptList.end())
	{
		std::cout << "Failed to RemoveScript\n";
		return;
	}
	m_scriptList.erase(state);
}


bool CompareData(const ScriptComponent & lhs, const ScriptComponent & rhs)
{
	UNREFERENCED_PARAMETER(lhs);
	UNREFERENCED_PARAMETER(rhs);
	return true;
}