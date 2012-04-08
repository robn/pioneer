#ifndef _UI_CONTEXT_H
#define _UI_CONTEXT_H

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

private:
	Graphics::Renderer *m_renderer;

	UI::Renderer *m_uiRenderer;
	UI::InputHandler *m_uiInputHandler;

	Gwen::Skin::TexturedBase *m_skin;
	Gwen::Controls::Canvas *m_canvas;
};

}

#endif
