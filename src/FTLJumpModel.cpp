#include "FTLJumpModel.h"
#include "Ship.h"
#include "Body.h"

FTLJumpModel::FTLJumpModel(Ship *ship, Body *target) :
	m_ship(ship),
	m_target(target)
{
	printf("ship %s ftl jump to %s\n", m_ship->GetLabel().c_str(), m_target->GetLabel().c_str());
}
