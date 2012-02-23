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

	Margin *createMargin(float margin) { return new Margin(this, margin); };
	Background *createBackground(const Color &color) { return new Background(this, color); }
	VBox *createVBox() { return new VBox(this); }
	HBox *createHBox() { return new HBox(this); }
	Image *createImage(const std::string &filename, Image::StretchMode stretchMode = Image::STRETCH_PRESERVE) { return new Image(this, filename, stretchMode); }
	Label *createLabel(const std::string &text) { return new Label(this, text); }

	RefCountedPtr<TextureFont> GetFont() { return m_font; }

private:
	RefCountedPtr<TextureFont> m_font;
};

}

#endif
