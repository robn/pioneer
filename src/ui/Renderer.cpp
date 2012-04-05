#include "ui/Renderer.h"
#include "graphics/Renderer.h"
#include "graphics/VertexArray.h"
#include "graphics/Material.h"
#include "graphics/Texture.h"
#include "graphics/TextureBuilder.h"
#include "FontCache.h"
#include "TextureFont.h"
#include "TextSupport.h"

#include <list>
#include <cstdio>

#include "Gwen/Gwen.h"
#include "Gwen/Texture.h"

namespace UI {

void Renderer::Begin()
{
	m_renderer->SetOrthographicProjection(0, m_width, m_height, 0, -1.f, 1.f);
}

void Renderer::End()
{
}

void Renderer::SetDrawColor(Gwen::Color color)
{
	m_drawColor = Color(float(color.r)/255.f, float(color.g)/255.f, float(color.b)/255.f, float(color.a)/255.f);
}

void Renderer::DrawLine(int x, int y, int a, int b)
{
	printf("UI::Renderer: DrawLine x %d y %d a %d b %d\n", x, y, a, b);
}

void Renderer::DrawFilledRect(Gwen::Rect rect)
{
	// XXX I'd prefer a "clear region" method on the renderer, but I need to
	// look up what's possible at the GL/D3D level to support it

	Graphics::VertexArray va(Graphics::ATTRIB_POSITION);
	va.Add(vector3f(rect.x,        rect.y,        0));
	va.Add(vector3f(rect.x,        rect.y+rect.h, 0));
	va.Add(vector3f(rect.x+rect.w, rect.y,        0));
	va.Add(vector3f(rect.x+rect.w, rect.y+rect.h, 0));

	Graphics::Material mat;
	mat.diffuse = m_drawColor;
	mat.unlit = true;

	m_renderer->DrawTriangles(&va, &mat, Graphics::TRIANGLE_STRIP);
}

void Renderer::LoadTexture(Gwen::Texture *gwenTexture)
{
	Graphics::Texture *texture = Graphics::TextureBuilder::UI(gwenTexture->name.c_str()).GetOrCreateTexture(m_renderer, "ui");
	gwenTexture->data = reinterpret_cast<void*>(new RefCountedPtr<Graphics::Texture>(texture));
	gwenTexture->width = texture->GetDescriptor().dataSize.x;
	gwenTexture->height = texture->GetDescriptor().dataSize.y;
	gwenTexture->failed = false;
}

void Renderer::FreeTexture(Gwen::Texture *gwenTexture)
{
	RefCountedPtr<Graphics::Texture> *texture = reinterpret_cast<RefCountedPtr<Graphics::Texture>*>(gwenTexture->data);
	delete texture;
}

// XXX horrible horrors to convert gwen's native widechar strings to something
// more palatable.
static inline std::string wstring_to_string(const std::wstring &src)
{
	std::string dest;
	char buf[4];
	for (unsigned int i = 0; i < src.length(); i++) {
		int len = conv_wc_to_mb(src[i], buf);
		dest.append(buf, len);
	}
	return dest;
}

void Renderer::LoadFont(Gwen::Font *gwenFont)
{
	// XXX build a new fontconfig using requested size/weight and instantiate a font with it
	FontCache cache;
	RefCountedPtr<TextureFont> font = cache.GetTextureFont(wstring_to_string(gwenFont->facename).c_str());
	gwenFont->data = reinterpret_cast<RefCountedPtr<TextureFont>*>(new RefCountedPtr<TextureFont>(font));
}

void Renderer::FreeFont(Gwen::Font *gwenFont)
{
	RefCountedPtr<TextureFont> *font = reinterpret_cast<RefCountedPtr<TextureFont>*>(gwenFont->data);
	delete font;
}

Gwen::Point Renderer::MeasureText(Gwen::Font *gwenFont, const Gwen::UnicodeString &text)
{
	if (!gwenFont->data) LoadFont(gwenFont);
	RefCountedPtr<TextureFont> font = *(reinterpret_cast<RefCountedPtr<TextureFont>*>(gwenFont->data));

	float w, h;
	font->MeasureString(wstring_to_string(text).c_str(), w, h);

	return Gwen::Point(int(ceilf(w)), int(ceilf(h)));
}

void Renderer::RenderText(Gwen::Font* gwenFont, Gwen::Point pos, const Gwen::UnicodeString& text)
{
	if (!gwenFont->data) LoadFont(gwenFont);
	RefCountedPtr<TextureFont> font = *(reinterpret_cast<RefCountedPtr<TextureFont>*>(gwenFont->data));

	font->RenderString(m_renderer, wstring_to_string(text).c_str(), pos.x, pos.y);
}

}
