#ifndef _GUICONTEXT_H
#define _GUICONTEXT_H

#include "RefCounted.h"
#include "TextureFont.h"

namespace Gui {

// The GUI context holds resources that are shared by all widgets. Examples of
// such resources are fonts, default styles, textures and so on. Each widget
// recieves a pointer to its context when it is added to a container, and can
// access it using its GetContext() method. Widgets that are not in a
// container do not have a context.

class Context {
public:
	Context();

	RefCountedPtr<TextureFont> GetFont() { return m_font; }

private:
	RefCountedPtr<TextureFont> m_font;
};

}

#endif
