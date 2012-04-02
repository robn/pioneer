#include "GuiBackground.h"
#include "GuiContext.h"
#include "graphics/Renderer.h"
#include "graphics/VertexArray.h"
#include "graphics/Material.h"

namespace Gui {

void Background::Draw()
{
	const vector2f &size = GetSize();

	// XXX I'd prefer a "clear region" method on the renderer, but I need to
	// look up what's possible at the GL/D3D level to support it

	Graphics::VertexArray va(Graphics::ATTRIB_POSITION);
	va.Add(vector3f(0,      0,      0));
	va.Add(vector3f(0,      size.y, 0));
	va.Add(vector3f(size.x, 0,      0));
	va.Add(vector3f(size.x, size.y, 0));

	Graphics::Material mat;
	mat.diffuse = m_color;
	mat.unlit = true;

	GetContext()->GetRenderer()->DrawTriangles(&va, &mat, Graphics::TRIANGLE_STRIP);

	Container::Draw();
}

}
