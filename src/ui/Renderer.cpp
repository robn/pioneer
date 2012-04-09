#include "ui/Renderer.h"
#include "graphics/Renderer.h"
#include "graphics/VertexArray.h"
#include "graphics/Material.h"
#include "graphics/Texture.h"
#include "graphics/TextureBuilder.h"
#include "text/TextureFont.h"
#include "text/FontDescriptor.h"
#include "text/TextSupport.h"

#include <list>
#include <cstdio>

#include "Gwen/Gwen.h"
#include "Gwen/Texture.h"

namespace UI {

void Renderer::Begin()
{
	m_renderer->SetOrthographicProjection(0, m_width, m_height, 0, -1.f, 1.f);
	m_renderer->SetDepthTest(false);
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
	Translate(rect);

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
	texture->IncRefCount();
	gwenTexture->data = reinterpret_cast<void*>(texture);
	gwenTexture->width = texture->GetDescriptor().dataSize.x;
	gwenTexture->height = texture->GetDescriptor().dataSize.y;
	gwenTexture->failed = false;
}

void Renderer::FreeTexture(Gwen::Texture *gwenTexture)
{
	Graphics::Texture *texture = reinterpret_cast<Graphics::Texture*>(gwenTexture->data);
	texture->DecRefCount();
}

void Renderer::DrawTexturedRect(Gwen::Texture* gwenTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2)
{
	Translate(rect);

	Graphics::Texture *texture = reinterpret_cast<Graphics::Texture*>(gwenTexture->data);

	Graphics::VertexArray va(Graphics::ATTRIB_POSITION | Graphics::ATTRIB_UV0);
	va.Add(vector3f(rect.x,        rect.y,        0.0f), vector2f(u1, v1));
	va.Add(vector3f(rect.x,        rect.y+rect.h, 0.0f), vector2f(u1, v2));
	va.Add(vector3f(rect.x+rect.w, rect.y,        0.0f), vector2f(u2, v1));
	va.Add(vector3f(rect.x+rect.w, rect.y+rect.h, 0.0f), vector2f(u2, v2));

	Graphics::Material mat;
	mat.unlit = true;
	mat.texture0 = texture;
	mat.vertexColors = false;

	m_renderer->DrawTriangles(&va, &mat, Graphics::TRIANGLE_STRIP);
}

// XXX horrible horrors to convert gwen's native widechar strings to something
// more palatable.
static inline std::string wstring_to_string(const std::wstring &src)
{
	std::string dest;
	char buf[4];
	for (unsigned int i = 0; i < src.length(); i++) {
		int len = Text::conv_wc_to_mb(src[i], buf);
		dest.append(buf, len);
	}
	return dest;
}

void Renderer::LoadFont(Gwen::Font *gwenFont)
{
	// XXX handle points, bold
	Text::FontDescriptor descriptor(wstring_to_string(gwenFont->facename), gwenFont->size, gwenFont->size);
	Text::TextureFont *font = new Text::TextureFont(descriptor, m_renderer);
	font->IncRefCount();
	gwenFont->data = reinterpret_cast<Text::TextureFont*>(font);
}

void Renderer::FreeFont(Gwen::Font *gwenFont)
{
	Text::TextureFont *font = reinterpret_cast<Text::TextureFont*>(gwenFont->data);
	font->DecRefCount();
}

Gwen::Point Renderer::MeasureText(Gwen::Font *gwenFont, const Gwen::UnicodeString &text)
{
	if (!gwenFont->data) LoadFont(gwenFont);
	Text::TextureFont *font = reinterpret_cast<Text::TextureFont*>(gwenFont->data);

	float w, h;
	font->MeasureString(wstring_to_string(text).c_str(), w, h);

	return Gwen::Point(int(ceilf(w)), int(ceilf(h)));
}

void Renderer::RenderText(Gwen::Font* gwenFont, Gwen::Point pos, const Gwen::UnicodeString& text)
{
	Translate(pos.x,pos.y);

	if (!gwenFont->data) LoadFont(gwenFont);
	Text::TextureFont *font = reinterpret_cast<Text::TextureFont*>(gwenFont->data);

	font->RenderString(wstring_to_string(text).c_str(), pos.x, pos.y, m_drawColor);
}

Gwen::Color Renderer::PixelColour(Gwen::Texture *gwenTexture, unsigned int x, unsigned int y, const Gwen::Color &defaultColor)
{
	Graphics::Texture *texture = reinterpret_cast<Graphics::Texture*>(gwenTexture->data);

	Color4ub c(texture->PixelColor(vector2f(x,y)));
	return Gwen::Color(c.r,c.g,c.b,c.a);
}

void Renderer::StartClip()
{
	Gwen::Rect rect = ClipRegion();

	vector2f pos = vector2f(rect.x, m_height - (rect.y+rect.h)) * Scale();
	vector2f size = vector2f(rect.w, rect.h) * Scale();

	m_renderer->SetScissor(true, pos, size);
}

void Renderer::EndClip()
{
	m_renderer->SetScissor(false);
}

}
