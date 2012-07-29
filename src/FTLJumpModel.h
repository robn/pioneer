#ifndef _FTLJUMPMODEL_H
#define _FTLJUMPMODEL_H

class Ship;
class Body;

class FTLJumpModel {
public:
	FTLJumpModel(Ship *ship, Body *target);

	void TimeStep(float step);

	Body *GetTarget() const { return m_target; }

	double GetTimeRemaining() const { return m_timeRemaining; }

private:
	Ship *m_ship;
	Body *m_target;

	double m_timeRemaining;
};

#endif
