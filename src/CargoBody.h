// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _CARGOBODY_H
#define _CARGOBODY_H

#include "libs.h"
#include "DynamicBody.h"
#include "EquipType.h"

namespace Graphics { class Renderer; }

class CargoBody: public DynamicBody {
public:
	OBJDEF(CargoBody, DynamicBody, CARGOBODY);
	CargoBody(Equip::Type t);
	CargoBody() {}

	virtual SaveLoad::Object Save(SaveLoad::SaveContext *sc) const;
	CargoBody(const SaveLoad::Object &so, SaveLoad::LoadContext *sc);

	Equip::Type GetCargoType() const { return m_type; }
	virtual void Render(Graphics::Renderer *r, const Camera *camera, const vector3d &viewCoords, const matrix4x4d &viewTransform);
	virtual bool OnCollision(Object *o, Uint32 flags, double relVel);
	virtual bool OnDamage(Object *attacker, float kgDamage);
protected:
/* XXX DESERIALIZER
	virtual void Load(SaveLoad::Reader &rd, Space *space);
*/
private:
	void Init();
	Equip::Type m_type;
	float m_hitpoints;
};

#endif /* _CARGOBODY_H */
