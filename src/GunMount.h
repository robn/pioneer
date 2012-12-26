// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _GUNMOUNT_H
#define _GUNMOUNT_H
#include "vector3.h"

class Body;
class Ship;
struct GunMountData;
struct TurretData;
enum Equip::Type;

class GunMount
{
  public:
	GunMount() {}
	GunMount(Ship *parent, const GunMountData &mount);

	bool IsFiring() const { return m_firing; }
	void SetFiring(bool firing) { m_firing = firing; }
	virtual void Update(float timeStep);			// timestep process

	void SetWeapon(Equip::Type weapontype) { m_weapontype = weapontype; }
	void SetCoolRate(float coolrate) { m_coolrate = coolrate; }

	float GetTemperature() const { return m_temperature; }
	virtual const vector3d &GetDir() const { return m_mount->dir; }

	virtual void Save(Serializer::Writer &wr);
	virtual void Load(Serializer::Reader &rd);

  protected:
	const GunMountData *m_mount;
	Equip::Type m_weapontype;
	float m_coolrate;		// based on laser cooling booster stuff? buff for stations?
// XXX forgot to do this bit

	bool m_firing;			// needs saving
	float m_temperature;	// needs saving
	float m_recharge;		// needs saving
	
	Ship *m_parent;			// passed in constructor, used when creating 	
							// can be used to get pos/dir/size
};

class Turret : public GunMount
{
  public:
	Turret() {}
	Turret(Ship *parent, const TurretData &turret);

	virtual void Update(float timeStep);			// timestep process

	void SetSkill(float skill) { m_skill = skill; }
	void SetTarget(Body *target) { m_target = target; m_leadTime = 0.0; }
	void OnDeleted(const Body *body) { if (body == m_target) m_target = 0; }

	virtual const vector3d &GetDir() const { return m_curdir; }

	virtual void Save(Serializer::Writer &wr);
	virtual void Load(Serializer::Reader &rd);

  private:
	const TurretData *m_turret;
	double m_dotextent;
	vector3d m_curdir;
	vector3d m_curvel;
	Body *m_target;

	float m_skill;				// skill of turret controller
	double m_leadTime;			// time to next update target heading
	vector3d m_leadOffset;
	vector3d m_leadDrift;
};

#endif // _GUNMOUNT_H


