#ifndef _GUICONTEXT_H
#define _GUICONTEXT_H

#include "RefCounted.h"
#include "TextureFont.h"

#include "GuiMargin.h"
#include "GuiBackground.h"
#include "GuiBox.h"
#include "GuiImage.h"
#include "GuiLabel.h"

namespace Gui {

// The GUI context holds resources that are shared by all widgets. Examples of
// such resources are fonts, default styles, textures and so on. New widgets
// are created from a context, and can access their context by calling their
// GetContext() method.

class Context {
public:
	Context();

	// general purpose containers
	Gui::HBox *HBox() { return new Gui::HBox(this); }
	Gui::VBox *VBox() { return new Gui::VBox(this); }

	// single containers
	Gui::Background *Background(const Color &color) { return new Gui::Background(this, color); }
	Gui::Margin *Margin(float margin) { return new Gui::Margin(this, margin); };

	// visual elements
	Gui::Image *Image(const std::string &filename, Image::StretchMode stretchMode = Image::STRETCH_PRESERVE) { return new Gui::Image(this, filename, stretchMode); }
	Gui::Label *Label(const std::string &text) { return new Gui::Label(this, text); }

	RefCountedPtr<TextureFont> GetFont() { return m_font; }

private:
	RefCountedPtr<TextureFont> m_font;
};

}

#endif
