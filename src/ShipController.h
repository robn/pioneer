// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _SHIPCONTROLLER_H
#define _SHIPCONTROLLER_H
/*
 * Ship movement controller class
 * Controls thrusters, autopilot according to player input or AI
 */
#include "libs.h"
#include "SaveLoad.h"
#include "Body.h"

class Ship;
class Space;

// only AI
class ShipController
{
public:
    enum FlightControlState { // <enum scope=ShipController name=ShipControllerFlightControlState prefix=CONTROL_>
        CONTROL_MANUAL,
        CONTROL_FIXSPEED,
        CONTROL_FIXHEADING_FORWARD,
        CONTROL_FIXHEADING_BACKWARD,
        CONTROL_AUTOPILOT,

        CONTROL_STATE_COUNT // <enum skip>
    };

	ShipController() { }
	virtual ~ShipController() { }

	virtual SaveLoad::Object Save(SaveLoad::SaveContext *sc) const;

/* XXX DESERIALIZER
	virtual void Load(SaveLoad::Reader &rd) { }
	virtual void PostLoadFixup(Space *) { }
*/
	virtual void StaticUpdate(float timeStep);
	virtual void SetFlightControlState(FlightControlState s) { }
	Ship *m_ship;
};

// autopilot AI + input
class PlayerShipController : public ShipController
{
public:
	PlayerShipController();
	~PlayerShipController();

	SaveLoad::Object Save(SaveLoad::SaveContext *sc) const;
/* XXX DESERIALIZER
	void Load(SaveLoad::Reader &rd);
	void PostLoadFixup(Space *s);
*/
	void StaticUpdate(float timeStep);
	// Poll controls, set thruster states, gun states and target velocity
	void PollControls(float timeStep, const bool force_rotation_damping);
	bool IsMouseActive() const { return m_mouseActive; }
	double GetSetSpeed() const { return m_setSpeed; }
	FlightControlState GetFlightControlState() const { return m_flightControlState; }
	vector3d GetMouseDir() const { return m_mouseDir; }
	void SetMouseForRearView(bool enable) { m_invertMouse = enable; }
	void SetFlightControlState(FlightControlState s);
	float GetLowThrustPower() const { return m_lowThrustPower; }
	void SetLowThrustPower(float power);

	bool GetRotationDamping() const { return m_rotationDamping; }
	void SetRotationDamping(bool enabled);
	void ToggleRotationDamping();
	void FireMissile();

	//targeting
	//XXX AI should utilize one or more of these
	Body *GetCombatTarget() const;
	Body *GetNavTarget() const;
	Body *GetSetSpeedTarget() const;
	void SetCombatTarget(Body* const target, bool setSpeedTo = false);
	void SetNavTarget(Body* const target, bool setSpeedTo = false);

	sigc::signal<void> onRotationDampingChanged;

private:
	bool IsAnyAngularThrusterKeyDown();
	bool IsAnyLinearThrusterKeyDown();
	//do a variety of checks to see if input is allowed
	void CheckControlsLock();
	Body* m_combatTarget;
	Body* m_navTarget;
	Body* m_setSpeedTarget;
	bool m_controlsLocked;
	bool m_invertMouse; // used for rear view, *not* for invert Y-axis option (which is Pi::IsMouseYInvert)
	bool m_mouseActive;
	bool m_rotationDamping;
	double m_mouseX;
	double m_mouseY;
	double m_setSpeed;
	FlightControlState m_flightControlState;
	float m_fovY; //for mouse acceleration adjustment
	float m_joystickDeadzone;
	float m_lowThrustPower;
	int m_combatTargetIndex; //for PostLoadFixUp
	int m_navTargetIndex;
	int m_setSpeedTargetIndex;
	vector3d m_mouseDir;

	sigc::connection m_connRotationDampingToggleKey;
	sigc::connection m_fireMissileKey;
};

#endif
