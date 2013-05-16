// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "CargoBody.h"
#include "Game.h"
#include "Pi.h"
#include "SaveLoad.h"
#include "Sfx.h"
#include "Space.h"
#include "EnumStrings.h"
#include "collider/collider.h"
#include "scenegraph/SceneGraph.h"

SaveLoad::Object CargoBody::Save(SaveLoad::SaveContext *sc) const {
	SaveLoad::Object so(DynamicBody::Save(sc));
	so.Set("bodyClass", "CargoBody");
	so.Set("cargoType", EnumStrings::GetString("EquipType", m_type));
	so.Set("hitpoints", m_hitpoints);
	return so;
}

CargoBody::CargoBody(const SaveLoad::Object &so, SaveLoad::LoadContext *lc): DynamicBody(so, lc)
{
	std::string cargoType;
	so.Get("cargoType", cargoType);
	m_type = static_cast<Equip::Type>(EnumStrings::GetValue("EquipType", cargoType.c_str()));

	so.Get("hitpoints", m_hitpoints);

	Init();
}

void CargoBody::Init()
{
	m_hitpoints = 1.0f;
	SetLabel(Equip::types[m_type].name);
	SetMassDistributionFromModel();

	std::vector<Color4ub> colors;
	//metallic blue-orangeish color scheme
	colors.push_back(Color4ub(255, 198, 64));
	colors.push_back(Color4ub(0, 222, 255));
	colors.push_back(Color4ub(255, 255, 255));
	GetModel()->SetColors(colors);

	Properties().Set("type", EnumStrings::GetString("EquipType", m_type));
}

CargoBody::CargoBody(Equip::Type t)
{
	m_type = t;
	SetModel("cargo");
	Init();
	SetMass(1.0);
}

bool CargoBody::OnDamage(Object *attacker, float kgDamage)
{
	m_hitpoints -= kgDamage*0.001f;
	if (m_hitpoints < 0) {
		Pi::game->GetSpace()->KillBody(this);
		Sfx::Add(this, Sfx::TYPE_EXPLOSION);
	}
	return true;
}

bool CargoBody::OnCollision(Object *b, Uint32 flags, double relVel)
{
	// ignore collision if its about to be scooped
	// XXX this is wrong. should only ignore if its actually going to be scooped. see Ship::OnCollision
	if (b->IsType(Object::SHIP)) {
		return true;
	}

	return DynamicBody::OnCollision(b, flags, relVel);
}

void CargoBody::Render(Graphics::Renderer *r, const Camera *camera, const vector3d &viewCoords, const matrix4x4d &viewTransform)
{
	GetModel()->SetLabel(Equip::types[m_type].name);
	RenderModel(r, camera, viewCoords, viewTransform);
}
