#ifndef _GRAPHIC_H
#define _GRAPHIC_H

#include "matrix4x4.h"
#include "SmartPtr.h"
#include "graphics/Material.h"
#include "graphics/Drawables.h"
#include "EquipType.h"

namespace Graphics {
	class Renderer;
}

class Graphic {
public:
	virtual ~Graphic() {}

	Graphics::Renderer *GetRenderer() const { return m_renderer; }

	void SetTransform(const matrix4x4d &transform) { m_transform = transform; }
	const matrix4x4d &GetTransform() const { return m_transform; }

	virtual void Draw() = 0;

protected:
	Graphic(Graphics::Renderer *r) : m_renderer(r), m_transform(matrix4x4d::Identity()) {}

private:
	Graphics::Renderer *m_renderer;
	matrix4x4d m_transform;
};

class ShieldGraphic : public Graphic {
public:
	ShieldGraphic(Graphics::Renderer *r);

	void SetStrength(float strength);

	void Draw();

private:
	ScopedPtr<Graphics::Drawables::Sphere3D> m_sphere;
};

class ECMGraphic : public Graphic {
public:
	ECMGraphic(Graphics::Renderer *r);

	void SetStrength(float strength);
	void SetRadius(float radius) { m_radius = radius; }

	void Draw();

private:
	RefCountedPtr<Graphics::Material> m_material;
	float m_radius;
};


class LaserGraphic : public Graphic {
public:
	LaserGraphic(Graphics::Renderer *r);

	void Draw();

	void SetLaserType(const LaserType &laserType) { m_laserType = laserType; }
	void SetAge(float age) { m_age = age; }

private:
	ScopedPtr<Graphics::VertexArray> m_sideVerts;
	ScopedPtr<Graphics::VertexArray> m_glowVerts;
	ScopedPtr<Graphics::Material> m_sideMat;
	ScopedPtr<Graphics::Material> m_glowMat;

	LaserType m_laserType;
	float m_age;
};
#endif
