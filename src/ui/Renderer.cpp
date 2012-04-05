#include "ui/Renderer.h"

namespace UI {

void Renderer::Begin()
{
}

void Renderer::End()
{
}

void Renderer::SetDrawColor(Gwen::Color color)
{
}

void Renderer::DrawLine(int x, int y, int a, int b)
{
}

void Renderer::DrawFilledRect(Gwen::Rect rect)
{
}

void Renderer::LoadTexture(Gwen::Texture *texture)
{
}

void Renderer::FreeTexture(Gwen::Texture *texture)
{
}

void Renderer::LoadFont(Gwen::Font *font)
{
}

void Renderer::FreeFont(Gwen::Font *font)
{
}

Gwen::Point Renderer::MeasureText(Gwen::Font *font, const Gwen::UnicodeString &text)
{
	return Gwen::Point();
}

}
