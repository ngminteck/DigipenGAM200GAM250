#include "Script.h"

Script::Script(std::string path,sol::state *state)
	:state{state},
	 pathname {path},
	 env{ *state,sol::create,state->globals() }
{
}

Script::~Script()
{
	state = nullptr;
}

sol::environment& Script::GetEnvironment()
{
	return env;
}


std::string Script::GetPath()
{
	return pathname;
}