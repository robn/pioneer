#include "GuiContext.h"

namespace Gui {

Context::Context() :
	m_font(RefCountedPtr<TextureFont>(new TextureFont(FontConfig("data/fonts/GuiFont.ini"), vector2f(1.0f,1.0f))))
{
}

}
