#include "libs.h"
#include "Gui.h"
#include "graphics/Graphics.h"

namespace GuiExtra {

namespace RawEvents {
	sigc::signal<void, GuiExtra::MouseMotionEvent *> onMouseMotion;
	sigc::signal<void, GuiExtra::MouseButtonEvent *> onMouseDown;
	sigc::signal<void, GuiExtra::MouseButtonEvent *> onMouseUp;
	sigc::signal<void, SDL_KeyboardEvent *> onKeyDown;
	sigc::signal<void, SDL_KeyboardEvent *> onKeyUp;
	sigc::signal<void, SDL_JoyAxisEvent *> onJoyAxisMotion;
	sigc::signal<void, SDL_JoyButtonEvent *> onJoyButtonDown;
	sigc::signal<void, SDL_JoyButtonEvent *> onJoyButtonUp;
	sigc::signal<void, SDL_JoyHatEvent *> onJoyHatMotion;
}

namespace Theme {
	namespace Colors {
		const float bg[] = { .25f,.37f,.63f };
		const float bgShadow[] = { .08f,.12f,.21f };
		const float tableHeading[] = { .7f,.7f,1.0f };
	}
	static const float BORDER_WIDTH = 2.0;

	void DrawRoundEdgedRect(const float size[2], float rad)
	{
		const int STEPS = 6;
		if (rad > 0.5f*std::min(size[0], size[1])) rad = 0.5f*std::min(size[0], size[1]);
		glBegin(GL_TRIANGLE_FAN);
			// top left
			// bottom left
			for (int i=0; i<=STEPS; i++) {
				float ang = M_PI*0.5f*i/float(STEPS);
				glVertex2f(rad - rad*cos(ang), (size[1] - rad) + rad*sin(ang));
			}
			// bottom right
			for (int i=0; i<=STEPS; i++) {
				float ang = M_PI*0.5 + M_PI*0.5f*i/float(STEPS);
				glVertex2f(size[0] - rad - rad*cos(ang), (size[1] - rad) + rad*sin(ang));
			}
			// top right
			for (int i=0; i<=STEPS; i++) {
				float ang = M_PI + M_PI*0.5f*i/float(STEPS);
				glVertex2f((size[0] - rad) - rad*cos(ang), rad + rad*sin(ang));
			}
			
			// top right
			for (int i=0; i<=STEPS; i++) {
				float ang = M_PI*1.5 + M_PI*0.5f*i/float(STEPS);
				glVertex2f(rad - rad*cos(ang), rad + rad*sin(ang));
			}
		glEnd();
	}
	
	void DrawHollowRect(const float size[2])
	{
		GLfloat vertices[] = { 0,0,
			0,size[1],
			size[0],size[1],
			size[0],0,
			BORDER_WIDTH,BORDER_WIDTH,
			BORDER_WIDTH,size[1]-BORDER_WIDTH,
			size[0]-BORDER_WIDTH,size[1]-BORDER_WIDTH,
			size[0]-BORDER_WIDTH,BORDER_WIDTH };
		GLubyte indices[] = {
			0,1,5,4, 0,4,7,3,
			3,7,6,2, 1,2,6,5 };
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glDrawElements(GL_QUADS, 16, GL_UNSIGNED_BYTE, indices);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void DrawIndent(const float size[2])
	{
		GLfloat vertices[] = { 0,0,
			0,size[1],
			size[0],size[1],
			size[0],0,
			BORDER_WIDTH,BORDER_WIDTH,
			BORDER_WIDTH,size[1]-BORDER_WIDTH,
			size[0]-BORDER_WIDTH,size[1]-BORDER_WIDTH,
			size[0]-BORDER_WIDTH,BORDER_WIDTH };
		GLubyte indices[] = {
			0,1,5,4, 0,4,7,3,
			3,7,6,2, 1,2,6,5,
			4,5,6,7 };
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glColor3fv(Colors::bgShadow);
		glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, indices);
		glColor3f(.6f,.6f,.6f);
		glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, indices+8);
		glColor3fv(Colors::bg);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices+16);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void DrawOutdent(const float size[2])
	{
		GLfloat vertices[] = { 0,0,
			0,size[1],
			size[0],size[1],
			size[0],0,
			BORDER_WIDTH,BORDER_WIDTH,
			BORDER_WIDTH,size[1]-BORDER_WIDTH,
			size[0]-BORDER_WIDTH,size[1]-BORDER_WIDTH,
			size[0]-BORDER_WIDTH,BORDER_WIDTH };
		GLubyte indices[] = {
			0,1,5,4, 0,4,7,3,
			3,7,6,2, 1,2,6,5,
			4,5,6,7 };
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glColor3f(.6f,.6f,.6f);
		glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, indices);
		glColor3fv(Colors::bgShadow);
		glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, indices+8);
		glColor3fv(Colors::bg);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices+16);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

}
