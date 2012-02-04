#include "Gui.h"
#include "GuiContext.h"
#include "vector3.h"		// for projection

namespace Gui {

Screen::Screen(Context *context, int real_width, int real_height, int ui_width, int ui_height) :
	m_context(context)
{
	width = ui_width;
	height = ui_height;
	realWidth = real_width;
	realHeight = real_height;
	invRealWidth = 1.0f/real_width;
	invRealHeight = 1.0f/real_height;
	// Why? because although our font textures get bigger with screen
	// resolution, our Gui Ortho projection is still 800x600 so vertex
	// coords must be scaled.
	fontScale[0] = ui_width / float(real_width);
	fontScale[1] = ui_height / float(real_height);
	baseContainer = new Gui::Fixed();
	baseContainer->SetContext(context);
	baseContainer->SetSize(float(Screen::width), float(Screen::height));
	baseContainer->Show();
}

Screen::~Screen()
{
	baseContainer->RemoveAllChildren();		// children deleted elsewhere?
	delete baseContainer;
}

void Screen::OnDeleteFocusedWidget()
{
	_focusedWidgetOnDelete.disconnect();
	focusedWidget = 0;
	SDL_EnableKeyRepeat(0, 0); // disable key repeat
}

void Screen::SetFocused(Widget *w, bool enableKeyRepeat)
{
	ClearFocus();
	if (enableKeyRepeat)
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	_focusedWidgetOnDelete = w->onDelete.connect(sigc::mem_fun(this, &Screen::OnDeleteFocusedWidget));
	focusedWidget = w;
}

void Screen::ClearFocus()
{
	if (!focusedWidget) return;
	_focusedWidgetOnDelete.disconnect();
	focusedWidget = 0;
	SDL_EnableKeyRepeat(0, 0); // disable key repeat
}

void Screen::ShowBadError(const char *msg)
{
	fprintf(stderr, "%s", msg);
	baseContainer->HideChildren();
	
	Gui::Fixed *f = new Gui::Fixed(6*GetWidth()/8.0f, 6*GetHeight()/8.0f);
	AddBaseWidget(f, GetWidth()/8, GetHeight()/8);
	f->SetTransparency(false);
	f->SetBgColor(0.4,0,0,1.0);
	f->Add(new Gui::Label(msg), 10, 10);

	Gui::Button *okButton = new Gui::LabelButton(new Gui::Label("Ok"));
	okButton->SetShortcut(SDLK_RETURN, KMOD_NONE);
	f->Add(okButton, 10.0f, 6*GetHeight()/8.0f - 32);
	f->ShowAll();
	f->Show();

	do {
		GetContext()->MainLoopIteration();
		SDL_Delay(10);
	} while (!okButton->IsPressed());

	RemoveBaseWidget(f);
	delete f;
	baseContainer->ShowAll();
}

bool Screen::Project(const vector3d &in, vector3d &out)
{
	GLint o = gluProject(in.x, in.y, in.z, modelMatrix, projMatrix, viewport, &out.x, &out.y, &out.z);
	out.x = out.x * width * invRealWidth;
	out.y = GetHeight() - out.y * height * invRealHeight;
	return (o == GL_TRUE);
}

void Screen::EnterOrtho()
{
	glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv (GL_VIEWPORT, viewport);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Screen::LeaveOrtho()
{	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void Screen::Draw()
{
	EnterOrtho();
	baseContainer->Draw();
	LeaveOrtho();
}

bool Screen::IsBaseWidget(const Widget *w)
{
	return w == static_cast<const Widget*>(baseContainer);
}

void Screen::AddBaseWidget(Widget *w, int x, int y)
{
	baseContainer->Add(w, float(x), float(y));
	w->SetContext(m_context);
}

void Screen::RemoveBaseWidget(Widget *w)
{
	baseContainer->Remove(w);
}
	
void Screen::SDLEventCoordToScreenCoord(int sdlev_x, int sdlev_y, float *x, float *y)
{
	*y = sdlev_y*height*invRealHeight;
	*x = sdlev_x*width*invRealWidth;
}

void Screen::OnMouseMotion(SDL_MouseMotionEvent *e)
{
	GuiExtra::MouseMotionEvent ev;
	float x, y;
	SDLEventCoordToScreenCoord(e->x, e->y, &x, &y);
	ev.screenX = ev.x = x;
	ev.screenY = ev.y = y;
	baseContainer->OnMouseMotion(&ev);
	ev.screenX = ev.x = x;
	ev.screenY = ev.y = y;
	GuiExtra::RawEvents::onMouseMotion.emit(&ev);
}

void Screen::OnClick(SDL_MouseButtonEvent *e)
{
	GuiExtra::MouseButtonEvent ev;
	float x, y;
	SDLEventCoordToScreenCoord(e->x, e->y, &x, &y);
	ev.button = e->button;
	ev.isdown = (e->type == SDL_MOUSEBUTTONDOWN);
	ev.screenX = ev.x = x;
	ev.screenY = ev.y = y;
	if (ev.isdown) {
		baseContainer->OnMouseDown(&ev);
		GuiExtra::RawEvents::onMouseDown.emit(&ev);
	} else {
		baseContainer->OnMouseUp(&ev);
		GuiExtra::RawEvents::onMouseUp.emit(&ev);
	}
}

void Screen::OnKeyDown(const SDL_keysym *sym)
{
	if (focusedWidget) {
		bool accepted = focusedWidget->OnKeyPress(sym);
		// don't check shortcuts if the focused widget accepted the key-press
		if (accepted)
			return;
	}
	for (std::list<Widget*>::iterator i = kbshortcut_widgets.begin(); i != kbshortcut_widgets.end(); ++i) {
		if (!(*i)->IsVisible()) continue;
		if (!(*i)->GetEnabled()) continue;
		(*i)->OnPreShortcut(sym);
	}
}

void Screen::OnKeyUp(const SDL_keysym *sym)
{
}

float Screen::GetFontHeight(TextureFont *font)
{
	if (!font) font = GetContext()->GetFont();

	return font->GetHeight() * fontScale[1];
}

void Screen::MeasureString(const std::string &s, float &w, float &h, TextureFont *font)
{
	if (!font) font = GetContext()->GetFont();
	assert(font);

	font->MeasureString(s.c_str(), w, h);
	w *= fontScale[0];
	h *= fontScale[1];
}

void Screen::MeasureCharacterPos(const std::string &s, int charIndex, float &x, float &y, TextureFont *font)
{
	assert((charIndex >= 0) && (charIndex <= int(s.size())));

	if (!font) font = GetContext()->GetFont();
	assert(font);

	font->MeasureCharacterPos(s.c_str(), charIndex, x, y);
	x *= fontScale[0];
	y *= fontScale[1];
}

int Screen::PickCharacterInString(const std::string &s, float x, float y, TextureFont *font)
{
	if (!font) font = GetContext()->GetFont();
	assert(font);

	x /= fontScale[0];
	y /= fontScale[1];

	return font->PickCharacter(s.c_str(), x, y);
}

void Screen::RenderString(const std::string &s, float xoff, float yoff, TextureFont *font)
{
	if (!font) font = GetContext()->GetFont();

	GLdouble modelMatrix_[16];
	glPushMatrix();
	glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix_);
	float x = modelMatrix_[12] + xoff;
	float y = modelMatrix_[13] + yoff;
	glLoadIdentity();
	glTranslatef(floor(x/fontScale[0])*fontScale[0],
			floor(y/fontScale[1])*fontScale[1], 0);
	glScalef(fontScale[0], fontScale[1], 1);
	font->RenderString(s.c_str(), 0, 0);
	glPopMatrix();
}

void Screen::RenderMarkup(const std::string &s, TextureFont *font)
{
    if (!font) font = GetContext()->GetFont();

	GLdouble modelMatrix_[16];
	glPushMatrix();
	glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix_);
	float x = modelMatrix_[12];
	float y = modelMatrix_[13];
	glLoadIdentity();
	glTranslatef(floor(x/fontScale[0])*fontScale[0],
			floor(y/fontScale[1])*fontScale[1], 0);
	glScalef(fontScale[0], fontScale[1], 1);
	font->RenderMarkup(s.c_str(), 0, 0);
	glPopMatrix();
}

void Screen::AddShortcutWidget(Widget *w)
{
	kbshortcut_widgets.push_back(w);
}

void Screen::RemoveShortcutWidget(Widget *w)
{
	kbshortcut_widgets.remove(w);
}

}
