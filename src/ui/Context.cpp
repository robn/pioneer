#include "Context.h"
#include "Renderer.h"
#include "Gwen/Gwen.h"
#include "Gwen/Skins/TexturedBase.h"
#include <SDL/SDL_events.h>

namespace UI {

Context::Context(Graphics::Renderer *renderer, int width, int height) : m_renderer(renderer)
{
	m_uiRenderer = new UI::Renderer(m_renderer, width, height);

	m_skin = new Gwen::Skin::TexturedBase(m_uiRenderer);
	m_skin->Init("textures/ui_texture.png");
	m_skin->SetDefaultFont(L"TitilliumText22L004.otf", 35.0f);

	m_canvas = new Gwen::Controls::Canvas(m_skin);
	m_canvas->SetSize(width, height);
}

void Context::Draw()
{
	m_canvas->RenderCanvas();
}

bool Context::ProcessEvent(const SDL_Event &event)
{
	switch(event.type) {
		case SDL_KEYUP:
		case SDL_KEYDOWN: {
			const SDL_KeyboardEvent &e = event.key;
			bool pressed = e.state == SDL_PRESSED;

			if (pressed && e.keysym.unicode)
				return m_canvas->InputCharacter(e.keysym.unicode);

			int gwenKey = -1;
			switch (e.keysym.sym) {
				case SDLK_RETURN:    gwenKey = Gwen::Key::Return;    break;
				case SDLK_BACKSPACE: gwenKey = Gwen::Key::Backspace; break;
				case SDLK_DELETE:    gwenKey = Gwen::Key::Delete;    break;
				case SDLK_LEFT:      gwenKey = Gwen::Key::Left;      break;
				case SDLK_RIGHT:     gwenKey = Gwen::Key::Right;     break;
				case SDLK_LSHIFT:    gwenKey = Gwen::Key::Shift;     break;
				case SDLK_RSHIFT:    gwenKey = Gwen::Key::Shift;     break;
				case SDLK_TAB:       gwenKey = Gwen::Key::Tab;       break;
				case SDLK_SPACE:     gwenKey = Gwen::Key::Space;     break;
				case SDLK_HOME:      gwenKey = Gwen::Key::Home;      break;
				case SDLK_END:       gwenKey = Gwen::Key::End;       break;
				case SDLK_LCTRL:     gwenKey = Gwen::Key::Control;   break;
				case SDLK_RCTRL:     gwenKey = Gwen::Key::Control;   break;
				case SDLK_UP:        gwenKey = Gwen::Key::Up;        break;
				case SDLK_DOWN:      gwenKey = Gwen::Key::Down;      break;
				case SDLK_ESCAPE:    gwenKey = Gwen::Key::Escape;    break;
				case SDLK_LALT:      gwenKey = Gwen::Key::Alt;       break;
				case SDLK_RALT:      gwenKey = Gwen::Key::Alt;       break;
				default: return false;
			}

			return m_canvas->InputKey(gwenKey, pressed);
		}

		case SDL_MOUSEMOTION: {
			const SDL_MouseMotionEvent &e = event.motion;
			return m_canvas->InputMouseMoved(e.x, e.y, e.xrel, e.yrel);
		}

		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN: {
			const SDL_MouseButtonEvent &e = event.button;

			if (e.button == SDL_BUTTON_WHEELUP || e.button == SDL_BUTTON_WHEELDOWN)
				return m_canvas->InputMouseWheel(e.button == SDL_BUTTON_WHEELUP ? -10 : 10); // XXX make delta configurable somewhere

			bool pressed = e.state == SDL_PRESSED;

			int button;
			switch (e.button) {
				case SDL_BUTTON_LEFT:   button = 0; break;
				case SDL_BUTTON_MIDDLE: button = 2; break;
				case SDL_BUTTON_RIGHT:  button = 1; break;
				default: return false;
			}

			return m_canvas->InputMouseButton(button, pressed);
		}

		default:
			return false;
	}
}

}
