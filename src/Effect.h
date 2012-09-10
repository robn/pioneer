#ifndef _EFFECT_H
#define _EFFECT_H

#include "vector3.h"
#include "SmartPtr.h"
#include "graphics/Drawables.h"

namespace Graphics {
	class Renderer;
}

class Effect {
};

class ShieldEffect : public Effect {
public:
	ShieldEffect(Graphics::Renderer *r);

	void SetPosition(const vector3d &pos) { m_pos = pos; }
	void SetSize(float size) { m_size = size; }
	void SetStrength(float strength);

	void Draw();

private:
	Graphics::Renderer *m_renderer;
	vector3d m_pos;
	float m_size;
	ScopedPtr<Graphics::Drawables::Sphere3D> m_sphere;
};

#endif
