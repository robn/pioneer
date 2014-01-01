// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "libs.h"
#include "graphics/Renderer.h"
#include "graphics/Material.h"

class Particle {
public:
	bool     enabled;
	vector3f pos;
	Color    color;
	float    energy;
};

class ParticleSystem {
public:
	ParticleSystem(Graphics::Renderer *r, RefCountedPtr<Graphics::Material> material, float spawnFrequency);
	virtual ~ParticleSystem() {}

	Particle *SpawnParticle();
	virtual void ResetParticle(Particle &p);

	void TimeStepUpdate(const float timeStep);
	void Draw(const matrix4x4f &trans);

private:
	Graphics::Renderer *m_renderer;
	RefCountedPtr<Graphics::Material> m_material;
	Graphics::RenderState *m_renderState;

	float m_spawnFrequency;

	float m_spawnTime;

	std::vector<Particle> m_particles;

	std::vector<vector3f> m_positions;
	std::vector<Color> m_colors;
};

#endif
