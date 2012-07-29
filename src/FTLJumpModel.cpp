#include "FTLJumpModel.h"
#include "Ship.h"
#include "Body.h"

FTLJumpModel::FTLJumpModel(Ship *ship, Body *target) :
	m_ship(ship),
	m_target(target)
{
	// XXX make a real calculation here
	m_timeRemaining = m_ship->GetPositionRelTo(m_target).Length() / AU;

	printf("ship %s ftl jump to %s in %f seconds\n", m_ship->GetLabel().c_str(), m_target->GetLabel().c_str(), m_timeRemaining);
}

void FTLJumpModel::TimeStep(float step) {
	if (m_timeRemaining > 0.0)
	    m_timeRemaining -= step;

	if (m_timeRemaining <= 0.0) {
		// XXX jump time
	}
}
