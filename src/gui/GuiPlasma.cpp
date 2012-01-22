#include "Gui.h"
#include "GuiPlasma.h"
#include "perlin.h"

namespace Gui {

Plasma::Plasma(unsigned int width, unsigned int height) :
	Widget(),
	m_width(width),
	m_height(height)
{
	SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE, m_width, m_height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);

	for (unsigned int x = 0; x < m_width; x++) {
		for (unsigned int y = 0; y < m_height; y++) {
			double n = (noise(double(x)/double(m_width), double(y)/double(m_height), 0.0) + 1.0) * 0.5; // scale noise() -1.0..1.0 to 0.0..1.0
			Uint8 intensity = Uint8(n * (256.0-FLT_EPSILON)); // scale to 0-255
			(reinterpret_cast<Uint32*>(s->pixels))[y*(s->pitch>>2)+x] = /*intensity + intensity*0x100*/ + intensity*0x10000 + 0xff000000;
		}
	}

	m_texture.Reset(new UITexture(s));

	SDL_FreeSurface(s);
}

void Plasma::Draw()
{
	float size[2];
	GetSize(size);

	m_texture->DrawUIQuad(size[0], size[1]);
}

void Plasma::GetSizeRequested(float size[2])
{
	size[0] = float(m_width);
	size[1] = float(m_height);
}

}
