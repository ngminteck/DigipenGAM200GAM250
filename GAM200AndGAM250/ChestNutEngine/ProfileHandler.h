#pragma once
#include <vector>

struct Profiler;

struct ProfileHandler
{
	ProfileHandler(std::vector<Profiler*>);
	~ProfileHandler();
	void Update();
	void Print();

	std::vector<Profiler *> m_profilers;
	double m_totaltime;
};