#include "Effect.h"

#include "matrix4x4.h"
#include "Color.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"

ShieldEffect::ShieldEffect(Graphics::Renderer *r) : Effect(),
	m_renderer(r),
	m_pos(vector3f(0.0f,0.0f,0.0f)),
	m_size(0.0f)
{
	Graphics::MaterialDescriptor desc;
	RefCountedPtr<Graphics::Material> mat(m_renderer->CreateMaterial(desc));
	m_sphere.Reset(new Graphics::Drawables::Sphere3D(mat, 2));

	SetStrength(1.0f);
}

void ShieldEffect::SetStrength(float strength)
{
	m_sphere->GetMaterial()->diffuse = Color((1.0f-strength), strength, 0.0, 0.33f*(1.0f-strength));
}

void ShieldEffect::Draw()
{
	m_renderer->SetBlendMode(Graphics::BLEND_ADDITIVE);
	glPushMatrix();
	matrix4x4d trans = matrix4x4d::Identity();
	trans.Translate(m_pos);
	trans.Scale(m_size);
	m_renderer->SetTransform(trans);

	//fade based on strength
	m_sphere->Draw(m_renderer);
	glPopMatrix();
	m_renderer->SetBlendMode(Graphics::BLEND_SOLID);
}
