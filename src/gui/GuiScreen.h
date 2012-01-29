#ifndef _GUISCREEN_H
#define _GUISCREEN_H

#include "Gui.h"
#include "FontManager.h"
#include "TextureFont.h"
#include "TextureCache.h"
#include <list>
#include <stack>

namespace Gui {

class Context;

class Screen {
public:
	Screen(Context *context, int real_width, int real_height, int ui_width, int ui_height);
	virtual ~Screen();

	Context *GetContext() const { return m_context; }

	void Draw();
	void ShowBadError(const char *msg);
	void AddBaseWidget(Widget *w, int x, int y);
	void RemoveBaseWidget(Widget *w);
	void OnMouseMotion(SDL_MouseMotionEvent *e);
	void OnClick(SDL_MouseButtonEvent *e);
	void OnKeyDown(const SDL_keysym *sym);
	void OnKeyUp(const SDL_keysym *sym);
	void EnterOrtho();
	void LeaveOrtho();
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	// gluProject but fixes UI/screen size mismatch
	bool Project(const vector3d &in, vector3d &out);
	friend void Widget::SetShortcut(SDLKey key, SDLMod mod);
	friend Widget::~Widget();
	bool IsBaseWidget(const Widget *);
	void GetCoords2Pixels(float scale[2]) {
		scale[0] = fontScale[0];
		scale[1] = fontScale[1];
	}
	const float* GetCoords2Pixels() { return fontScale; }
	void SetFocused(Widget *w, bool enableKeyRepeat = false);
	void ClearFocus();
	bool IsFocused(Widget *w) {
		return w == focusedWidget;
	}

	float GetFontHeight(TextureFont *font = 0);
	void RenderString(const std::string &s, float xoff, float yoff, TextureFont *font = 0);
	void MeasureString(const std::string &s, float &w, float &h, TextureFont *font = 0);
	int PickCharacterInString(const std::string &s, float x, float y, TextureFont *font = 0);
	void MeasureCharacterPos(const std::string &s, int charIndex, float &x, float &y, TextureFont *font = 0);
	void RenderMarkup(const std::string &s, TextureFont *font = 0);

private:
	void AddShortcutWidget(Widget *w);
	void RemoveShortcutWidget(Widget *w);
	void SDLEventCoordToScreenCoord(int sdlev_x, int sdlev_y, float *x, float *y);

	Context *m_context;

	int width, height;
	int realWidth, realHeight;
	float invRealWidth, invRealHeight;
	std::list<Widget*> kbshortcut_widgets;
	std::list<Widget*> mouseHoveredWidgets;
	float fontScale[2];
	Gui::Fixed *baseContainer;
	Gui::Widget *focusedWidget;
	void OnDeleteFocusedWidget();
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	sigc::connection _focusedWidgetOnDelete;
};

}

#endif /* _GUISCREEN_H */
