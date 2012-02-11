#include "GuiBackground.h"

namespace Gui {

void Background::Draw()
{
	const vector2f &pos = GetPosition();
	const vector2f &size = GetSize();

	GLfloat array[4*2] = {
		pos.x,        pos.y+size.y,
		pos.x+size.x, pos.y+size.y,
		pos.x+size.x, pos.y,
		pos.x,        pos.y
	};

	glColor4fv(m_color);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(GLfloat)*2, &array[0]);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	Container::Draw();
}

}
