#pragma once
#include "Component.h"
#include "SceneActor.h"
#include "Script.h"
#include "BehaviourTree.h"
#include <unordered_map>
#include <string>


class ScriptComponent : public Component
{
private:
	std::unordered_map<std::string, Script*> m_scriptList;
	sol::state *m_luaState;
public:
	ScriptComponent(sol::state*);
	ScriptComponent(const ScriptComponent &);
	~ScriptComponent();
	Component * Clone() const;

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	Script* GetScript(std::string state);
	sol::state* GetLuaState();

	void InitScripts(SceneActor*, SceneActor* player);	// Function called when in "start" state
	void AddScript(std::string state, std::string scriptpath);
	void RemoveScript(std::string state);
	std::unordered_map<std::string, Script *> * GetScriptList()  { return &m_scriptList; }

	constexpr static Metadata <ScriptComponent, std::unordered_map<std::string, Script*> ScriptComponent::*> metadata
	{ "Sprite Component", std::make_tuple(&ScriptComponent::m_scriptList), std::make_tuple("m_scriptList") };
};

bool CompareData(const ScriptComponent & lhs, const ScriptComponent & rhs);