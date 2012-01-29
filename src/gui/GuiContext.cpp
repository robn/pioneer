#include "Gui.h"
#include "GuiContext.h"
#include "render/Render.h"

namespace Gui {

Context::Context(int screen_width, int screen_height, int ui_width, int ui_height)
{
	m_fontManager.Reset(new FontManager);
	m_defaultFont = m_fontManager->GetTextureFont("GuiFont");
	PushFont(m_defaultFont);

	m_textureCache.Reset(new TextureCache);

	SDL_EnableUNICODE(1);
	screen = new Screen(this, screen_width, screen_height, ui_width, ui_height);
}

Context::~Context()
{
	for (std::list<TimerSignal*>::iterator i = m_timeSignals.begin(); i != m_timeSignals.end(); ++i)
		delete (*i);
	delete screen;
}

void Context::HandleSDLEvent(SDL_Event *event)
{
	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			screen->OnClick(&event->button);
			break;
		case SDL_MOUSEBUTTONUP:
			screen->OnClick(&event->button);
			break;
		case SDL_KEYDOWN:
			screen->OnKeyDown(&event->key.keysym);
			GuiExtra::RawEvents::onKeyDown.emit(&event->key);
			break;
		case SDL_KEYUP:
			screen->OnKeyUp(&event->key.keysym);
			GuiExtra::RawEvents::onKeyUp.emit(&event->key);
			break;
		case SDL_MOUSEMOTION:
			screen->OnMouseMotion(&event->motion);
			break;
		case SDL_JOYAXISMOTION:
			GuiExtra::RawEvents::onJoyAxisMotion(&event->jaxis);
			break;
		case SDL_JOYBUTTONUP:
			GuiExtra::RawEvents::onJoyButtonUp(&event->jbutton);
			break;
		case SDL_JOYBUTTONDOWN:
			GuiExtra::RawEvents::onJoyButtonDown(&event->jbutton);
			break;
		case SDL_JOYHATMOTION:
			GuiExtra::RawEvents::onJoyHatMotion(&event->jhat);
			break;
	}
}

void Context::Draw()
{
	Uint32 t = SDL_GetTicks();
	// also abused like an update() function...
	for (std::list<TimerSignal*>::iterator i = m_timeSignals.begin(); i != m_timeSignals.end();) {
		if (t >= (*i)->goTime) {
			(*i)->sig.emit();
			delete *i;
			i = m_timeSignals.erase(i);
		} else {
			++i;
		}
	}
//	ExpireTimers(t);

	screen->Draw();
}

void Context::MainLoopIteration()
{
	Render::PrepareFrame();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// handle events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		HandleSDLEvent(&event);
		if (event.type == SDL_QUIT) {
			SDL_Quit();
			exit(0);
		}
	}

	SDL_ShowCursor(1);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	Render::PostProcess();
	Draw();
	Render::SwapBuffers();
}

sigc::connection Context::AddTimer(Uint32 ms, sigc::slot<void> slot)
{
	TimerSignal *s = new TimerSignal(SDL_GetTicks() + ms);
	sigc::connection con = s->sig.connect(slot);
	m_timeSignals.push_back(s);
	return con;
}

}
