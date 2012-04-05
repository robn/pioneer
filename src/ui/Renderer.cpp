#include "ui/Renderer.h"
#include "graphics/Renderer.h"
#include "graphics/VertexArray.h"
#include "graphics/Material.h"

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

void Renderer::LoadTexture(Gwen::Texture *texture)
{
	printf("UI::Renderer: LoadTexture name %s w %d h %d\n", texture->name.c_str(), texture->width, texture->height);
}

void Renderer::FreeTexture(Gwen::Texture *texture)
{
	printf("UI::Renderer: FreeTexture name %s\n", texture->name.c_str());
}

void Renderer::LoadFont(Gwen::Font *font)
{
	printf("UI::Renderer: LoadFont facename %ls size %f bold %s\n", font->facename.c_str(), font->size, font->bold ? "YES" : "NO");
}

void Renderer::FreeFont(Gwen::Font *font)
{
	printf("UI::Renderer: FreeFont facename %ls\n", font->facename.c_str());
}

Gwen::Point Renderer::MeasureText(Gwen::Font *font, const Gwen::UnicodeString &text)
{
	printf("UI::Renderer: MeasureText facename %ls text %ls\n", font->facename.c_str(), text.c_str());
	return Gwen::Point();
}

}
