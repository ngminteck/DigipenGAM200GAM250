#pragma once
#include "Component.h"
#include <vector>

class StatsComponent : public Component
{
private:
	float m_HP;
	float m_MP;
	
	std::string m_char;
	bool m_isDead;
	bool m_invul;
	float m_invulTimer;

	Vector2D m_respawnPos;

	std::vector<std::tuple<std::string, float, float, bool>> m_cooldownVec;
public:
	StatsComponent();
	StatsComponent(float hp, float mp, std::string charac, bool isDead, bool invul, float invulTimer);
	~StatsComponent();
	Component * Clone() const override;

	void SetHP(float val);
	void SetMP(float val);
	void SetChar(std::string str);
	void SetDead(bool val);
	void SetInvul(bool val);
	void SetInvulTimer(float val);
	void SetRespawnPos(Vector2D pos);

	float GetHP() const;
	float GetMP() const;
	std::string GetChar() const;
	bool GetDead() const;
	bool GetInvul() const;
	float GetInvulTimer() const;
	Vector2D GetRespawnPos() const;

	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	void RegisterCooldown(const std::string name, const float& value, const float& timer, const bool & state);
	void UpdateCooldown(const float& value);
	void ActivateCooldown(const std::string name, bool state);
	void ResetCooldown(const std::string name);
	void SetCooldown(const std::string& name, float value);
	float GetCooldown(const std::string& name) const;
	bool IsActive(const std::string& name) const;
	void PrintAllCooldown() const;

	constexpr static Metadata < StatsComponent, float StatsComponent::*, float StatsComponent::*, bool StatsComponent::*, float StatsComponent::*> metadata
	{ "StatsComponent", std::make_tuple(&StatsComponent::m_HP, &StatsComponent::m_MP, &StatsComponent::m_invul, &StatsComponent::m_invulTimer), std::make_tuple("m_HP", "m_MP", "m_invul", "m_invulTimer") };

	static void AddToScript(sol::state* luaState);
};

bool CompareData(const StatsComponent & lhs, const StatsComponent & rhs);