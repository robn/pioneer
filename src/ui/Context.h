#ifndef _UI_CONTEXT_H
#define _UI_CONTEXT_H

#include "Gwen/Font.h"

namespace Graphics {
	class Renderer;
}
namespace Gwen {
	namespace Skin {
		class TexturedBase;
	}
	namespace Controls {
		class Canvas;
	}
}
union SDL_Event;

namespace UI {

class Renderer;
class InputHandler;

class Context {
public:
	Context(Graphics::Renderer *renderer, int width, int height);

	Gwen::Controls::Canvas *GetCanvas() const { return m_canvas; }

	void Draw();

	bool ProcessEvent(const SDL_Event &event);

	enum FontType {
		FONT_SMALL,
		FONT_TINY,
		FONT_NORMAL,
		FONT_LARGE,
		FONT_HUGE,

		FONT_MAX
	};
	Gwen::Font *GetFont(FontType type) { return &m_fonts[type]; }

private:
	Graphics::Renderer *m_renderer;

	UI::Renderer *m_uiRenderer;
	UI::InputHandler *m_uiInputHandler;

	Gwen::Skin::TexturedBase *m_skin;
	Gwen::Controls::Canvas *m_canvas;

	Gwen::Font m_fonts[FONT_MAX];
};

}

#endif
