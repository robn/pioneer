#ifndef _GUICONTEXT_H
#define _GUICONTEXT_H 1

#include "libs.h"
#include <stack>

class FontManager;
class TextureCache;
class TextureFont;

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


	FontManager *GetFontManager() { return m_fontManager.Get(); }

	void PushFont(TextureFont* f) { m_fontStack.push(f); }
	void PushFont(const std::string &name);
	void PopFont() { m_fontStack.pop(); };
	TextureFont *GetFont() { return m_fontStack.size() ? m_fontStack.top() : m_defaultFont; }
	TextureFont *GetDefaultFont() { return m_defaultFont; }

	TextureCache *GetTextureCache() { return m_textureCache.Get(); }


	Screen *screen;

private:
	struct TimerSignal {
		TimerSignal(Uint32 _goTime) : goTime(_goTime) {}
		Uint32 goTime;
		sigc::signal<void> sig;
	};
	std::list<TimerSignal*> m_timeSignals;

	ScopedPtr<FontManager> m_fontManager;
	std::stack<TextureFont*> m_fontStack;
	TextureFont *m_defaultFont;

	ScopedPtr<TextureCache> m_textureCache;
};

}

#endif
