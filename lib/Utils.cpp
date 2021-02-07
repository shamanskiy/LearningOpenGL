#include "Utils.h"

EventContainer::EventContainer() :
	m_timeTracker() {}

TimeTracker::TimeTracker() : 
	m_time(0.),
	m_timeStep(0.),
	m_canComputeTimeStep(false)
{}

void TimeTracker::setTime(GLfloat time)
{
	if (m_canComputeTimeStep)
		m_timeStep = time - m_time;
	else
		m_canComputeTimeStep = true;
	m_time = time;
}
