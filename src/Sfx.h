// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _SFX_H
#define _SFX_H

#include "Body.h"
#include "SaveLoad.h"

class Frame;
namespace Graphics {
	class Renderer;
	class Material;
	namespace Drawables {
		class Sphere3D;
	}
}

class Sfx {
public:
	enum Type { // <enum scope='Sfx' name=SfxType prefix=TYPE_>
		TYPE_NONE,
		TYPE_EXPLOSION,
		TYPE_DAMAGE,
		TYPE_SMOKE
	};

	static void Add(const Body *, Type);
	static void AddThrustSmoke(const Body *b, Type, float speed, vector3d adjustpos);
	static void TimeStepAll(const float timeStep, Frame *f);
	static void RenderAll(Graphics::Renderer *r, Frame *f, const Frame *camFrame);

	static SaveLoad::Object Save(const Frame *f);
/* XXX DESERIALIZER
	static void Unserialize(SaveLoad::Reader &rd, Frame *f);
*/

	Sfx();
	void SetPosition(const vector3d &p);
	vector3d GetPosition() const { return m_pos; }

	//create shared models
	static void Init(Graphics::Renderer *r);
	static void Uninit();
	static Graphics::Drawables::Sphere3D *shieldEffect;
	static Graphics::Drawables::Sphere3D *explosionEffect;
	static Graphics::Material *damageParticle;
	static Graphics::Material *ecmParticle;
	static Graphics::Material *smokeParticle;

private:
	static Sfx *AllocSfxInFrame(Frame *f);

	void Render(Graphics::Renderer *r, const matrix4x4d &transform);
	void TimeStepUpdate(const float timeStep);

	vector3d m_pos;
	vector3d m_vel;
	float m_age;
	float m_speed;
	enum Type m_type;
};

#endif /* _SFX_H */
