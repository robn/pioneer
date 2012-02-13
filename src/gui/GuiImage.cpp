#include "GuiImage.h"
#include "Texture.h"

namespace Gui {

Image::Image(const std::string &filename): Widget()
{
	m_texture.Reset(new UITexture(filename));
}

const Metrics &Image::GetMetrics()
{
	static const Metrics metrics(vector2f(m_texture->GetWidth(),m_texture->GetHeight()));
	return metrics;
}

void Image::Draw()
{
	glEnable(GL_BLEND);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	m_texture->DrawUIQuad(0,0,GetSize().x,GetSize().y);
	glDisable(GL_BLEND);
}

}
