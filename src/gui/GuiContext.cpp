#include "GuiContext.h"
#include "FileSystem.h"

namespace Gui {

Context::Context(Graphics::Renderer *renderer) :
	m_renderer(renderer),
	m_font(RefCountedPtr<TextureFont>(new TextureFont(renderer, FontConfig(FileSystem::JoinPathBelow(FileSystem::GetDataDir(), "fonts/GuiFont.ini")), vector2f(1.0f,1.0f))))
{
}

}
