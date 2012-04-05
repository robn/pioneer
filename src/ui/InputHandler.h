#ifndef _UI_INPUTHANDLER_H
#define _UI_INPUTHANDLER_H

#include <SDL/SDL_events.h>

namespace Gwen {
	namespace Controls {
		class Canvas;
	}
}

namespace UI {

class InputHandler {
public:
	InputHandler(Gwen::Controls::Canvas *canvas) : m_canvas(canvas) {}

	bool ProcessEvent(const SDL_Event &event);

private:
	Gwen::Controls::Canvas *m_canvas;
};

}

#endif
