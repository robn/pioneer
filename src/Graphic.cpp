#include "Graphic.h"

#include "matrix4x4.h"
#include "Color.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"

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
