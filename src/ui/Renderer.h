#ifndef _UI_RENDERER_H
#define _UI_RENDERER_H

#include "Gwen/BaseRender.h"

namespace Graphics { class Renderer; }

namespace UI {

class Renderer : public Gwen::Renderer::Base {
public:
	Renderer(Graphics::Renderer *renderer) : m_renderer(renderer) {}
	virtual ~Renderer() {}

	virtual void Begin();
	virtual void End();

	virtual void SetDrawColor(Gwen::Color color);

	virtual void DrawLine(int x, int y, int a, int b);
	virtual void DrawFilledRect(Gwen::Rect rect);

	virtual void LoadTexture(Gwen::Texture *texture);
	virtual void FreeTexture(Gwen::Texture *texture);

	virtual void LoadFont(Gwen::Font *font);
	virtual void FreeFont(Gwen::Font *font);

	virtual Gwen::Point MeasureText(Gwen::Font *font, const Gwen::UnicodeString &text);

private:
	Graphics::Renderer *m_renderer;
};

}

#endif
