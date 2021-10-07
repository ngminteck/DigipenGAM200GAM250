#include "Component.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent()
	:Component{ComponentType::MovementComponent },
	m_State_Logic{ "IDLE" }, 
	m_nextState_Logic{ "IDLE" }, 
	m_prevState_Logic{ "IDLE" },
	m_Dir { LEFT }, m_prevDir{ LEFT },
	m_timeCount_Logic{ 0.0f }
{
}
MovementComponent::MovementComponent(const MovementComponent & movComp)
	:Component(movComp), 
	m_State_Logic(movComp.m_State_Logic),
	m_nextState_Logic(movComp.m_nextState_Logic),
	m_prevState_Logic(movComp.m_prevState_Logic),
	m_Dir(movComp.m_Dir),
	m_timeCount_Logic(movComp.m_timeCount_Logic)
{ }
MovementComponent::MovementComponent(std::string s, int d)
	: Component{ ComponentType::MovementComponent },
	m_State_Logic{ s },
	m_prevState_Logic{ s },
	m_nextState_Logic{ s },
	m_Dir{ static_cast<Direction>(d) }, m_prevDir{ m_Dir },
	m_timeCount_Logic{ 0.0f }
{
}

MovementComponent::~MovementComponent()
{
}

Component * MovementComponent::Clone() const
{
	return new MovementComponent(*this);
}

std::string MovementComponent::GetPrevStateLogic() const
{
	return m_prevState_Logic;
}

std::string MovementComponent::GetStateLogic() const
{
	return m_State_Logic;
}

std::string MovementComponent::GetNextStateLogic() const
{
	return m_nextState_Logic;
}

Direction MovementComponent::GetDirection()const
{
	return m_Dir;
}

Direction MovementComponent::GetPrevDirection()const
{
	return m_prevDir;
}


float MovementComponent::GetTimeCountLogic() const
{
	return m_timeCount_Logic;
}

void MovementComponent::SetPrevStateLogic(std::string s)
{
	m_prevState_Logic = s;
}

void MovementComponent::SetPrevDirection(Direction d)
{
	m_prevDir = d;
}
void MovementComponent::SetDirection(Direction d)
{
	m_prevDir = m_Dir;
	m_Dir = d;
}

void MovementComponent::SetStateLogic(std::string s)
{
	m_prevState_Logic = m_State_Logic;
	m_State_Logic = s;
}

void MovementComponent::SetNextStateLogic(std::string s)
{
	m_nextState_Logic = s;
}



void MovementComponent::SetTimeCountLogic(float val)
{
	m_timeCount_Logic = val;
}

void MovementComponent::Unserialize(rapidjson::PrettyWriter<rapidjson::StringBuffer> & writer)
{
	writer.Key("ComponentType");
	writer.String("MovementComponent");
	auto map = metadata.reflect<TypeErasedObj>(*this);
	for (auto elem : map)
	{
		AutoWrite(writer, elem.first, elem.second.GetValue());
	}
}

void MovementComponent::AddToScript(sol::state * luaState)
{
	//luaState->new_enum("State",
	//	"Idle", State::IDLE,
	//	"Walk", State::WALK,
	//	"Dash", State::DASH,
	//	"Jump", State::JUMP,
	//	"Attack1", State::ATTACK,
	//	"Attack2", State::ATTACK_COMBO_1,
	//	"Attack3", State::ATTACK_COMBO_2,
	//	"Attack4", State::ATTACK_COMBO_3,
	//	"Flinch", State::FLINCH,
	//	"Dash", State::DASH_ATTACK,
	//	"Defend", State::DEFEND,
	//	"SpecialAttack", State::SPATTACK,
	//	"Transform", State::TRANSFORM
	//	);
	luaState->new_enum("Direction",
		"Left", Direction::LEFT,
		"Right", Direction::RIGHT
	);
	luaState->new_usertype<MovementComponent>("	",
		// Member functions
		"GetPrevLogicState"	, &MovementComponent::GetPrevStateLogic,
		"GetCurrLogicState"	, &MovementComponent::GetStateLogic,
		"GetNextLogicState"	, &MovementComponent::GetNextStateLogic,
		"SetPrevLogicState", &MovementComponent::SetPrevStateLogic,
		"SetCurrLogicState", &MovementComponent::SetStateLogic,
		"SetNextLogicState", &MovementComponent::SetNextStateLogic,

		"GetCurrDirection"	, &MovementComponent::GetDirection,
		"GetPrevDirection"	, &MovementComponent::GetPrevDirection,
		 										  
		"SetCurrDirection"	, &MovementComponent::SetDirection,
		"SetPrevDirection"	, &MovementComponent::SetPrevDirection,

		sol::base_classes, sol::bases<Component>()
		);
}

bool CompareData(const MovementComponent & lhs, const MovementComponent & rhs)
{
	UNREFERENCED_PARAMETER(lhs);
	UNREFERENCED_PARAMETER(rhs);
	return true;
}