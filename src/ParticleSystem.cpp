// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "ParticleSystem.h"

static const size_t NUM_PARTICLES = 256;

ParticleSystem::ParticleSystem(Graphics::Renderer *r, RefCountedPtr<Graphics::Material> material) :
	m_renderer(r),
	m_material(material),
	m_particles(NUM_PARTICLES),
	m_spritePositions(NUM_PARTICLES)
{
	for (size_t i = 0; i < NUM_PARTICLES; i++)
		ResetParticle(m_particles[i]);
		//m_particles[i].energy = 0;
}

inline float rang(int max) {
	return DEG2RAD(float((rand() % max) - (max/2)));
}

void ParticleSystem::ResetParticle(Particle &p)
{
	vector3f vel = vector3f(0.f,0.2f,0.f);
	vel.ArbRotate(vector3f(0.f,0.f,1.f), rang(360));
	vel.ArbRotate(vector3f(0.f,1.f,0.f), rang(360));
	vel.ArbRotate(vector3f(1.f,0.f,0.f), rang(360));
	p = {
		.pos    = vector3f(0.f),
		.oldPos = vector3f(0.f),
		.vel    = vel,
		.color  = Color::WHITE,
		.energy = 255,
		.size   = 1.f,
	};
}

void ParticleSystem::Update()
{
	for (size_t i = 0; i < NUM_PARTICLES; i++) {
		Particle &p = m_particles[i];
		if (p.energy == 0) continue;

		p.oldPos = p.pos;
		p.pos += p.vel;
		p.vel.ArbRotate(vector3f(0.f,0.f,1.f), rang(30));
		p.vel.ArbRotate(vector3f(0.f,1.f,0.f), rang(30));
		p.vel.ArbRotate(vector3f(1.f,0.f,0.f), rang(30));
		//p.energy -= 1;
	}
}

void ParticleSystem::Draw(const matrix4x4f &trans)
{
	size_t si = 0;
	for (size_t i = 0; i < NUM_PARTICLES; i++) {
		Particle &p = m_particles[i];
		if (p.energy == 0) continue;
		m_spritePositions[si++] = p.pos;
	}

	m_renderer->SetTransform(trans);
	m_renderer->SetBlendMode(Graphics::BLEND_ALPHA);
	m_renderer->DrawPointSprites(si, &m_spritePositions[0], m_material.Get(), 1.f);
}
