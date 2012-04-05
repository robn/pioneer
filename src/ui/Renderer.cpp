#include "ui/Renderer.h"

#include <list>
#include <cstdio>

#include "Gwen/Gwen.h"
#include "Gwen/Texture.h"

namespace UI {

void Renderer::Begin()
{
	printf("UI::Renderer: Begin\n");
}

void Renderer::End()
{
	printf("UI::Renderer: End\n");
}

void Renderer::SetDrawColor(Gwen::Color color)
{
	printf("UI::Renderer: SetDrawColor\n");
}

void Renderer::DrawLine(int x, int y, int a, int b)
{
	printf("UI::Renderer: DrawLine x %d y %d a %d b %d\n", x, y, a, b);
}

void Renderer::DrawFilledRect(Gwen::Rect rect)
{
	printf("UI::Renderer: DrawFilledRect x %d y %d w %d h %d\n", rect.x, rect.y, rect.w, rect.h);
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
