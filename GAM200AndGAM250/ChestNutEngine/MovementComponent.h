#pragma once
#include "Component.h"
#include "State.h"
class MovementComponent : public Component
{
private:
	std::string m_prevState_Logic; 
	std::string m_State_Logic;
	std::string m_nextState_Logic;

	Direction m_Dir;
	Direction m_prevDir;

	float m_timeCount_Logic;

public:
	MovementComponent();
	MovementComponent(const MovementComponent& movComp);
	MovementComponent(std::string,int);
	~MovementComponent();
	Component * Clone() const override;

	std::string GetPrevStateLogic() const;
	std::string GetStateLogic() const;
	std::string GetNextStateLogic() const;
	Direction GetDirection()const;
	Direction GetPrevDirection()const;

	float GetTimeCountLogic() const;

	void SetPrevStateLogic(std::string s);
	void SetStateLogic(std::string s);
	void SetNextStateLogic(std::string s);

	void SetPrevDirection(enum Direction);
	void SetDirection(enum Direction);

	void SetTimeCountLogic(float val);
	void Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer);

	
	constexpr static Metadata <MovementComponent, std::string MovementComponent::*, 
							   std::string MovementComponent::*, Direction MovementComponent::*> metadata
	{ "MovementComponent", std::make_tuple(&MovementComponent::m_State_Logic, &MovementComponent::m_nextState_Logic,
										   &MovementComponent::m_Dir) ,
						   std::make_tuple("m_State", "m_nextState", "m_Dir")	
	};

	static void AddToScript(sol::state * luaState);
};

bool CompareData(const MovementComponent & lhs, const MovementComponent & rhs);
