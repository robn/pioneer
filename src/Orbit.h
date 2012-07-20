#ifndef _ORBIT_H
#define _ORBIT_H

#include "matrix4x4.h"
#include "vector3.h"

class Orbit {
public:
	// eccentricity:  0.0 (circular) - 1.0 (parabolic)
	// semiMajorAxis: distance in metres
	// mass:          total mass of system (both objects) in Kg
	// position:      initial position as a rotation about the origin
	Orbit(double eccentricity, double semiMajorAxis, double mass, const matrix4x4d &position);

	Orbit() :
		m_eccentricity(0.0), m_semiMajorAxis(0.0), m_period(0.0), m_position(matrix4x4d::Identity()) {}

	vector3d OrbitalPosAtTime(double t) const;
	// 0.0 <= t <= 1.0. Not for finding orbital pos
	vector3d EvenSpacedPosAtTime(double t) const;

	double GetEccentricity() const { return m_eccentricity; }
	double GetSemiMajorAxis() const { return m_semiMajorAxis; }
	double GetPeriod() const { return m_period; }
	const matrix4x4d &GetInitialPosition() const { return m_position; }

private:
	double m_eccentricity;  // 0.0-1.0
	double m_semiMajorAxis; // metres
	double m_period;        // seconds

	matrix4x4d m_position;
};

#endif
