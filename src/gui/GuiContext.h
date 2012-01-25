#ifndef _GUICONTEXT_H
#define _GUICONTEXT_H 1

#include "libs.h"

namespace Gui {

class Screen;

class Context {
public:
	Context(int screen_width, int screen_height, int ui_width, int ui_height);
	~Context();

	void HandleSDLEvent(SDL_Event *event);
	void Draw();
	void MainLoopIteration();
	sigc::connection AddTimer(Uint32 ms, sigc::slot<void> slot);

	Screen *screen;

private:
	struct TimerSignal {
		TimerSignal(Uint32 _goTime) : goTime(_goTime) {}
		Uint32 goTime;
		sigc::signal<void> sig;
	};
	std::list<TimerSignal*> m_timeSignals;
};

}

#endif
