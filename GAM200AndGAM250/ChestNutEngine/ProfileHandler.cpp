#include "ProfileHandler.h"
#include "Profiler.h"
ProfileHandler::ProfileHandler(std::vector<Profiler *> profiles)
	:m_profilers{profiles}, m_totaltime{0.0}
{

}

ProfileHandler::~ProfileHandler()
{

}

void ProfileHandler::Print()
{
	system("cls");
	for(auto profile : m_profilers)
		std::cout << profile->system_name << " : " << profile->duration.count() << " : " << std::endl;
}

void ProfileHandler::Update()
{
	//Calculate total time
	m_totaltime = 0.0f;
	for (auto profile : m_profilers)
		m_totaltime += profile->duration.count();
}