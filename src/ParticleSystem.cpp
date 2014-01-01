// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "ParticleSystem.h"
#include "Easing.h"
#include "graphics/RenderState.h"

static const size_t NUM_PARTICLES = 256;

ParticleSystem::ParticleSystem(Graphics::Renderer *r, RefCountedPtr<Graphics::Material> material, float spawnFrequency) :
	m_renderer(r),
	m_material(material),
	m_spawnFrequency(spawnFrequency),
	m_spawnTime(0.f),
	m_particles(NUM_PARTICLES),
	m_positions(NUM_PARTICLES),
	m_colors(NUM_PARTICLES)
{
	for (size_t i = 0; i < NUM_PARTICLES; i++) {
		ResetParticle(m_particles[i]);
		//m_particles[i].energy = 0;
	}

	for (size_t i = 0; i < NUM_PARTICLES; i++)
		m_particles[i].enabled = false;

	Graphics::RenderStateDesc rsd;
	rsd.blendMode = Graphics::BLEND_ALPHA;
	rsd.depthWrite = false;
	rsd.cullMode = Graphics::CULL_NONE;
	m_renderState = r->CreateRenderState(rsd);
}

inline float rang(int max) {
	return DEG2RAD(float((rand() % max) - (max/2)));
}

void ParticleSystem::ResetParticle(Particle &p)
{
	p = {
		.enabled = true,
		.pos     = vector3f(0.f),
		.color   = Color(rand() % 255, rand() % 255, rand() % 255),
		.energy  = 100.0f,
	};
}

Particle *ParticleSystem::SpawnParticle()
{
	for (size_t i = 0; i < NUM_PARTICLES; i++) {
		Particle &p = m_particles[i];
		if (p.enabled) continue;
		return &p;
	}
	return nullptr;
}

void ParticleSystem::TimeStepUpdate(const float timeStep)
{
	m_spawnTime += timeStep;
    if (m_spawnTime >= m_spawnFrequency) {
		ResetParticle(*SpawnParticle());
		m_spawnTime = 0;
	}

	for (size_t i = 0; i < NUM_PARTICLES; i++) {
		Particle &p = m_particles[i];
		if (!p.enabled) continue;

		p.energy -= timeStep;
		if (p.energy <= 0) {
			p.enabled = false;
			continue;
		}

		p.color.a = (p.energy/100.0f) * 255;

		p.pos.x = Easing::Sine::EaseInOut(100.f-p.energy, 0.f, 4.f, 1.f) - 2.0f;
		p.pos.z = Easing::Sine::EaseInOut(100.5f-p.energy, 0.f, 4.f, 1.f) - 2.0f;

		p.pos.y = (100.f-p.energy)*2.f-8.f;
	}
}

void ParticleSystem::Draw(const matrix4x4f &trans)
{
	size_t si = 0;
	for (size_t i = 0; i < NUM_PARTICLES; i++) {
		Particle &p = m_particles[i];
		if (!p.enabled) continue;
		m_positions[si] = p.pos;
		m_colors[si] = p.color;
		si++;
	}

	m_renderer->SetTransform(trans);
	m_renderer->DrawPointSprites(si, &m_positions[0], &m_colors[0], nullptr, m_material.Get(), 1.f);
}
