#pragma once
#include <string>
#include <sol.hpp>
class Script
{
public:
	Script(std::string path,sol::state*);
	Script(const Script & ) = delete;
	~Script();

	sol::environment& GetEnvironment();
	std::string GetPath();
private:
	sol::state * state;
	std::string pathname;
	sol::environment env;
};