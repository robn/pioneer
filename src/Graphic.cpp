#include "Graphic.h"

#include "matrix4x4.h"
#include "Color.h"
#include "Pi.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"
#include "graphics/TextureBuilder.h"

ShieldGraphic::ShieldGraphic(Graphics::Renderer *r) : Graphic(r)
{
	Graphics::MaterialDescriptor desc;
	RefCountedPtr<Graphics::Material> mat(GetRenderer()->CreateMaterial(desc));
	m_sphere.Reset(new Graphics::Drawables::Sphere3D(mat, 2));

	SetStrength(1.0f);
}

void ShieldGraphic::SetStrength(float strength)
{
	m_sphere->GetMaterial()->diffuse = Color((1.0f-strength), strength, 0.0, 0.33f*(1.0f-strength));
}

void ShieldGraphic::Draw()
{
	GetRenderer()->SetBlendMode(Graphics::BLEND_ADDITIVE);
	m_sphere->Draw(GetRenderer());
	GetRenderer()->SetBlendMode(Graphics::BLEND_SOLID);
}

ECMGraphic::ECMGraphic(Graphics::Renderer *r) : Graphic(r)
{
	Graphics::MaterialDescriptor desc;
	desc.textures = 1;
	m_material.Reset(r->CreateMaterial(desc));
	m_material->texture0 = Graphics::TextureBuilder::Billboard("textures/ecm.png").GetOrCreateTexture(r, "billboard");
	m_material->diffuse = Color(0.5f, 0.5f, 1.0f, 0.0f); 

	SetStrength(1.0f);
	SetRadius(0.0f);
}

void ECMGraphic::SetStrength(float strength)
{
	m_material->diffuse.a = strength;
}

void ECMGraphic::Draw()
{
	vector3f v[100];
	for (int i=0; i<100; i++) {
		const float(r1) = Pi::rng.Double()-0.5;
		const float(r2) = Pi::rng.Double()-0.5;
		const float(r3) = Pi::rng.Double()-0.5;
		v[i] = vector3f(r1, r2, r3).Normalized() * m_radius;
	}
	GetRenderer()->DrawPointSprites(100, v, m_material.Get(), 50.0f);
}
