#include "Graphic.h"

#include "matrix4x4.h"
#include "Color.h"
#include "Pi.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"
#include "graphics/TextureBuilder.h"
#include "graphics/VertexArray.h"

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

LaserGraphic::LaserGraphic(Graphics::Renderer *r) : Graphic(r)
{
	//set up materials
	Graphics::MaterialDescriptor desc;
	desc.textures = 1;
	desc.twoSided = true;
	m_sideMat.Reset(Pi::renderer->CreateMaterial(desc));
	m_glowMat.Reset(Pi::renderer->CreateMaterial(desc));
	m_sideMat->texture0 = Graphics::TextureBuilder::Billboard("textures/projectile_l.png").GetOrCreateTexture(Pi::renderer, "billboard");
	m_glowMat->texture0 = Graphics::TextureBuilder::Billboard("textures/projectile_w.png").GetOrCreateTexture(Pi::renderer, "billboard");

	//zero at projectile position
	//+x down
	//+y right
	//+z forwards (or projectile direction)
	const float w = 0.5f;

	vector3f one(0.f, -w, 0.f); //top left
	vector3f two(0.f,  w, 0.f); //top right
	vector3f three(0.f,  w, -1.f); //bottom right
	vector3f four(0.f, -w, -1.f); //bottom left

	//uv coords
	const vector2f topLeft(0.f, 1.f);
	const vector2f topRight(1.f, 1.f);
	const vector2f botLeft(0.f, 0.f);
	const vector2f botRight(1.f, 0.f);

	m_sideVerts.Reset(new Graphics::VertexArray(Graphics::ATTRIB_POSITION | Graphics::ATTRIB_UV0));
	m_glowVerts.Reset(new Graphics::VertexArray(Graphics::ATTRIB_POSITION | Graphics::ATTRIB_UV0));

	//add four intersecting planes to create a volumetric effect
	for (int i=0; i < 4; i++) {
		m_sideVerts->Add(one, topLeft);
		m_sideVerts->Add(two, topRight);
		m_sideVerts->Add(three, botRight);

		m_sideVerts->Add(three, botRight);
		m_sideVerts->Add(four, botLeft);
		m_sideVerts->Add(one, topLeft);

		one.ArbRotate(vector3f(0.f, 0.f, 1.f), DEG2RAD(45.f));
		two.ArbRotate(vector3f(0.f, 0.f, 1.f), DEG2RAD(45.f));
		three.ArbRotate(vector3f(0.f, 0.f, 1.f), DEG2RAD(45.f));
		four.ArbRotate(vector3f(0.f, 0.f, 1.f), DEG2RAD(45.f));
	}

	//create quads for viewing on end
	float gw = 0.5f;
	float gz = -0.1f;

	for (int i=0; i < 4; i++) {
		m_glowVerts->Add(vector3f(-gw, -gw, gz), topLeft);
		m_glowVerts->Add(vector3f(-gw, gw, gz), topRight);
		m_glowVerts->Add(vector3f(gw, gw, gz), botRight);

		m_glowVerts->Add(vector3f(gw, gw, gz), botRight);
		m_glowVerts->Add(vector3f(gw, -gw, gz), botLeft);
		m_glowVerts->Add(vector3f(-gw, -gw, gz), topLeft);

		gw -= 0.1f; // they get smaller
		gz -= 0.2f; // as they move back
	}
}

void LaserGraphic::Draw()
{
	// increase visible size based on distance from camera
	// allows them to be smaller while maintaining visibility for game play
	const float dist_scale = GetTransform().GetTranslate().z / 500;
	const float length = m_laserType.length + dist_scale;
	const float width = m_laserType.width + dist_scale;
	GetRenderer()->SetTransform(GetTransform() * matrix4x4d::ScaleMatrix(width, width, length));

	GetRenderer()->SetBlendMode(Graphics::BLEND_ALPHA_ONE);
	GetRenderer()->SetDepthWrite(false);

	Color color = m_laserType.color;

	// fade them out as they age so they don't suddenly disappear
	// this matches the damage fall-off calculation
	const float base_alpha = sqrt(1.0f - m_age/m_laserType.lifespan);
	const float ang = 1.0f - fabs(vector3d(1.0,0.0,0.0).Dot(GetTransform().ApplyRotationOnly(vector3d(1.0,0.0,0.0))));

	// fade out side quads when viewing nearly edge on
	color.a = base_alpha * (1.f - powf(ang, length));
	if (color.a > 0.01f) {
		m_sideMat->diffuse = color;
		GetRenderer()->DrawTriangles(m_sideVerts.Get(), m_sideMat.Get());
	}

	// fade out glow quads when viewing nearly edge on
	// these and the side quads fade at different rates
	// so that they aren't both at the same alpha as that looks strange
	color.a = base_alpha * powf(ang, width);
	if (color.a > 0.01f) {
		m_glowMat->diffuse = color;
		GetRenderer()->DrawTriangles(m_glowVerts.Get(), m_glowMat.Get());
	}

	GetRenderer()->SetBlendMode(Graphics::BLEND_SOLID);
	GetRenderer()->SetDepthWrite(true);
}

HyperspaceCloudGraphic::HyperspaceCloudGraphic(Graphics::Renderer *r, const Color &color) : Graphic(r)
{
	m_vertices.Reset(new Graphics::VertexArray(Graphics::ATTRIB_POSITION | Graphics::ATTRIB_DIFFUSE));

	Graphics::MaterialDescriptor desc;
	desc.vertexColors = true;
	m_material.Reset(GetRenderer()->CreateMaterial(desc));

	Color c(color);
	c.a = 0.0f;
	m_vertices->Add(vector3f(0.f, 0.f, 0.f), Color::WHITE);
	for (int i = 72; i >= 0; i--)
		m_vertices->Add(vector3f( 0.f+sinf(DEG2RAD(i*5.f)), 0.f+cosf(DEG2RAD(i*5.f)), 0.f), c);
}

void HyperspaceCloudGraphic::Draw()
{
	GetRenderer()->SetTransform(GetTransform() * matrix4x4d::ScaleMatrix(m_radius));

	GetRenderer()->SetBlendMode(Graphics::BLEND_ALPHA_ONE);
	GetRenderer()->DrawTriangles(m_vertices.Get(), m_material.Get(), Graphics::TRIANGLE_FAN);
	GetRenderer()->SetBlendMode(Graphics::BLEND_SOLID);
}
