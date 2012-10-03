#ifndef _GRAPHIC_H
#define _GRAPHIC_H

#include "matrix4x4.h"
#include "SmartPtr.h"
#include "graphics/Drawables.h"

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

#endif
