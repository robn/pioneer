#include "GuiBackground.h"

namespace Gui {

void Background::Draw()
{
	const vector2f &size = GetSize();

	// XXX use renderer

	GLfloat array[4*2] = {
		0,      size.y,
		size.x, size.y,
		size.x, 0,
		0,      0
	};

	glColor4fv(m_color);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(GLfloat)*2, &array[0]);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	Container::Draw();
}

}
