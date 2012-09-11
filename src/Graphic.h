#ifndef _GRAPHIC_H
#define _GRAPHIC_H

#include "vector3.h"
#include "SmartPtr.h"
#include "graphics/Drawables.h"

namespace Graphics {
	class Renderer;
}

class Graphic {
public:
	virtual ~Graphic() {}

	Graphics::Renderer *GetRenderer() const { return m_renderer; }

	void SetPosition(const vector3d &pos) { m_pos = pos; }
	const vector3d &GetPosition() const { return m_pos; }

	virtual void Draw() = 0;

protected:
	Graphic(Graphics::Renderer *r) : m_renderer(r) {}

private:
	Graphics::Renderer *m_renderer;
	vector3d m_pos;
};

class ShieldGraphic : public Graphic {
public:
	ShieldGraphic(Graphics::Renderer *r);

	void SetSize(float size) { m_size = size; }
	void SetStrength(float strength);

	void Draw();

private:
	float m_size;
	ScopedPtr<Graphics::Drawables::Sphere3D> m_sphere;
};

#endif
