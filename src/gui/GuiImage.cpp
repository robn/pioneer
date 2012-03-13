#include "GuiImage.h"
#include "GuiContext.h"
#include "graphics/TextureBuilder.h"

namespace Gui {

Image::Image(Context *context, const std::string &filename, StretchMode stretchMode): Widget(context),
	m_stretchMode(stretchMode)
{
	Graphics::TextureBuilder b = Graphics::TextureBuilder::UI(filename);
	m_quad.Reset(new TexturedQuad(b.GetOrCreateTexture(GetContext()->GetRenderer(), "ui")));

	const Graphics::TextureDescriptor &descriptor = b.GetDescriptor();
	m_initialSize = vector2f(descriptor.dataSize.x*descriptor.texSize.x,descriptor.dataSize.y*descriptor.texSize.y);
}

Metrics Image::GetMetrics(const vector2f &hint)
{
	return Metrics(m_initialSize);
}

void Image::Update()
{
	vector2f size = GetSize();

	switch (m_stretchMode) {
		case STRETCH_MAX:
			m_scaledSize = size;
			break;

		case STRETCH_PRESERVE: {

			float originalRatio = m_initialSize.x / m_initialSize.y;
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
	Graphics::Renderer *r = GetContext()->GetRenderer();
	r->SetBlendMode(Graphics::BLEND_ALPHA);
	m_quad->Draw(r, 0, m_scaledSize);
}

}
