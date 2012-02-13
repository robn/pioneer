#include "GuiImage.h"
#include "Texture.h"

namespace Gui {

Image::Image(const std::string &filename, StretchMode stretchMode): Widget(),
	m_stretchMode(stretchMode)
{
	m_texture.Reset(new UITexture(filename));
}

Metrics Image::GetMetrics()
{
	return Metrics(vector2f(m_texture->GetWidth(),m_texture->GetHeight()));
}

void Image::Update()
{
	vector2f size = GetSize();

	switch (m_stretchMode) {
		case STRETCH_MAX:
			m_scaledSize = size;
			break;

		case STRETCH_PRESERVE: {

			float originalRatio = float(m_texture->GetWidth()) / float(m_texture->GetHeight());
			float wantRatio = size.x / size.y;

			// more room on X than Y, use full X, scale Y
			if (wantRatio < originalRatio) {
				m_scaledSize.x = size.x;
				m_scaledSize.y = size.x / originalRatio;
			}

			// more room on Y than X, use full Y, scale X
			else {
				m_scaledSize.x = size.y * originalRatio;
				m_scaledSize.y = size.y;
			}

			break;
		}
		
		default:
			assert(0);
	}
}

void Image::Draw()
{
	glEnable(GL_BLEND);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	m_texture->DrawUIQuad(0, 0, m_scaledSize.x, m_scaledSize.y);
	glDisable(GL_BLEND);
}

}
