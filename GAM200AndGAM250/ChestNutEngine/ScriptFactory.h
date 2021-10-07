#pragma once
#include "ScriptComponent.h"
#include "ComponentFactory.h"
#include "rapidjson/document.h"
class ScriptFactory : public ComponentFactory
{
private:
	sol::state * LuaState;
public:
	ScriptFactory(sol::state* luastate)
		:LuaState{luastate}
	{

	}
	~ScriptFactory()
	{

	}

	Component * CreateComponent() const
	{
		return new ScriptComponent(LuaState);
	}
	Component * CreateComponent(const rapidjson::Value& node)const
	{
		ScriptComponent * comp = new ScriptComponent{ LuaState };
		if (node.HasMember("Script"))
		{
			const rapidjson::Value& scriptNode = node["Script"];
			for (rapidjson::SizeType i = 0; i < scriptNode.Size(); ++i)
			{
				if (scriptNode[i].HasMember("Path") && scriptNode[i].HasMember("State"))
				{
					comp->AddScript(scriptNode[i]["State"].GetString(), scriptNode[i]["Path"].GetString());
				}
			}
		}
		return comp;
	}
};