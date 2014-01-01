// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "libs.h"
#include "graphics/Renderer.h"
#include "graphics/Material.h"

class Particle {
public:
	vector3f pos;
	vector3f oldPos;
	vector3f vel;
	Color    color;
	Uint32   energy;
	float    size;
};

class ParticleSystem {
public:
	ParticleSystem(Graphics::Renderer *r, RefCountedPtr<Graphics::Material> material);
	virtual ~ParticleSystem() {}

	virtual void ResetParticle(Particle &p);

	void Update();
	void Draw(const matrix4x4f &trans);

private:
	Graphics::Renderer *m_renderer;
	RefCountedPtr<Graphics::Material> m_material;

	std::vector<Particle> m_particles;

	std::vector<vector3f> m_spritePositions;
};

#endif
