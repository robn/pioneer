#ifndef _GUICONTEXT_H
#define _GUICONTEXT_H

#include "RefCounted.h"
#include "TextureFont.h"

#include "GuiMargin.h"
#include "GuiBackground.h"
#include "GuiBox.h"
#include "GuiImage.h"
#include "GuiLabel.h"

namespace Graphics { class Renderer; }

namespace Gui {

// The GUI context holds resources that are shared by all widgets. Examples of
// such resources are fonts, default styles, textures and so on. New widgets
// are created from a context, and can access their context by calling their
// GetContext() method.

class Context {
public:
	Context(Graphics::Renderer *renderer);

	// general purpose containers
	Gui::HBox *HBox() { return new Gui::HBox(this); }
	Gui::VBox *VBox() { return new Gui::VBox(this); }

	// single containers
	Gui::Background *Background(const Color &color) { return new Gui::Background(this, color); }
	Gui::Margin *Margin(float margin) { return new Gui::Margin(this, margin); };

	// visual elements
	Gui::Image *Image(const std::string &filename, Image::StretchMode stretchMode = Image::STRETCH_PRESERVE) { return new Gui::Image(this, filename, stretchMode); }
	Gui::Label *Label(const std::string &text) { return new Gui::Label(this, text); }

	Graphics::Renderer *GetRenderer() const { return m_renderer; }
	RefCountedPtr<TextureFont> GetFont() const { return m_font; }

private:
	Graphics::Renderer *m_renderer;
	RefCountedPtr<TextureFont> m_font;
};

}

#endif
