#include <cstdlib>
#include "SDL.h"
#include "FileSystem.h"
#include "OS.h"
#include "graphics/Graphics.h"
#include "graphics/Renderer.h"
#include "graphics/TextureBuilder.h"
#include "ParticleSystem.h"

static const int WIDTH  = 1024;
static const int HEIGHT = 768;

int main(int argc, char **argv)
{
	FileSystem::Init();

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "sdl init failed: %s\n", SDL_GetError());
		exit(-1);
	}

	Graphics::Settings videoSettings;
	videoSettings.width = WIDTH;
	videoSettings.height = HEIGHT;
	videoSettings.fullscreen = false;
	videoSettings.requestedSamples = 0;
	videoSettings.vsync = false;
	videoSettings.useTextureCompression = false;
	videoSettings.enableDebugMessages = false;
	videoSettings.iconFile = OS::GetIconFilename();
	videoSettings.title = "particletest";

	Graphics::Renderer *r = Graphics::Init(videoSettings);

    r->SetPerspectiveProjection(85, r->GetDisplayAspect(), 0.1f, 10000.f);
	r->SetTransform(matrix4x4f::Identity());
	r->SetClearColor(Color::BLACK);

	Graphics::MaterialDescriptor desc;
	desc.textures = 1;
	desc.vertexColors = true;
	Graphics::Material *mat = r->CreateMaterial(desc);
	mat->texture0 = Graphics::TextureBuilder::Billboard("textures/smoke.png").GetOrCreateTexture(r, "billboard");

	ParticleSystem *ps = new ParticleSystem(r, RefCountedPtr<Graphics::Material>(mat), 0.1f);

	const double TIMESTEP = 1.0/60.0;

	double currentTime = 0.001 * double(SDL_GetTicks());
	double frameTime = 0.0;
	double accumulator = TIMESTEP;

	while (1) {
		bool done = false;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				done = true;
		}
		if (done)
			break;

		const Uint32 newTicks = SDL_GetTicks();
		double newTime = 0.001 * double(newTicks);
		frameTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= TIMESTEP) {
			ps->TimeStepUpdate(TIMESTEP);
			accumulator -= TIMESTEP;
		}

		r->ClearScreen();
		ps->Draw(matrix4x4f::Translation(0.0f, 0.0f, -10.0f));
		r->SwapBuffers();
	}

	delete ps;

	SDL_Quit();

	exit(0);
}
