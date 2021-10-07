#include <chrono>
#include <iostream>
#include <string>
struct Profiler
{
	Profiler(std::string name) :system_name{ name } {}

	void UpdateStart()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	void UpdateEnd()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<milli>(end - start);
	}

	// std::chrono::milliseconds == std::chrono::duration<long long, std::milli>
	using milli = std::chrono::duration<double, std::milli>;

	std::string system_name;

	std::chrono::high_resolution_clock::time_point start; //start
	std::chrono::high_resolution_clock::time_point end; //end
	milli duration; //duration

};