#include <cstdlib>
#include "SDL.h"
#include "GuiContext.h"
#include "GuiScreen.h"
#include "GuiBackground.h"
#include "GuiBox.h"
#include "GuiImage.h"
#include "GuiLabel.h"
#include "GuiMargin.h"
#include "FileSystem.h"
#include "graphics/Graphics.h"
#include "graphics/Renderer.h"

static const int WIDTH  = 1024;
static const int HEIGHT = 768;

static bool click_handler(const Gui::MouseButtonEvent &event)
{
	printf("click: %f,%f\n", event.pos.x, event.pos.y);
	return true;
}

static bool move_handler(const Gui::MouseMotionEvent &event)
{
	printf("move: %f,%f\n", event.pos.x, event.pos.y);
	return true;
}

int main(int argc, char **argv)
{
	FileSystem::Init();

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "sdl init failed: %s\n", SDL_GetError());
		exit(-1);
	}

    const SDL_VideoInfo *info = SDL_GetVideoInfo();
    switch (info->vfmt->BitsPerPixel) {
        case 16:
            SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
            SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
            SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
            break;
        case 24:
        case 32:
            SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
            break;
        default:
            fprintf(stderr, "invalid pixel depth: %d bpp\n", info->vfmt->BitsPerPixel);
            exit(-1);
    }
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	SDL_Surface *surface = SDL_SetVideoMode(WIDTH, HEIGHT, info->vfmt->BitsPerPixel, SDL_OPENGL);
	if (!surface) {
		fprintf(stderr, "sdl video mode init failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(-1);
	}

	SDL_WM_SetCaption("guitest", "guitest");

	Graphics::Renderer *r = Graphics::Init(WIDTH, HEIGHT, true);
	r->SetOrthographicProjection(0, WIDTH, HEIGHT, 0, -1, 1);
	r->SetTransform(matrix4x4f::Identity());
	r->SetClearColor(Color::BLACK);

	// XXX GL renderer enables lighting by default. if all draws use materials
	// that's ok, but for filled regions (ie Background) its not right. a
	// scissored version of Renderer::ClearScreen would be the most efficient,
	// but I'm not quite ready to do it yet.
	glDisable(GL_LIGHTING);
	// XXX sorta ditto the depth test. should renderer start with these
	// disabled?
	glDisable(GL_DEPTH_TEST);

	Gui::Context *c = new Gui::Context(r);
	Gui::Screen *screen = new Gui::Screen(c, WIDTH, HEIGHT);

	Gui::Button *button;
	screen->SetInnerWidget(
		c->Margin(10.0f)->SetInnerWidget(
			(button = c->Button())
		)
	);

	button->onMouseUp.connect(sigc::ptr_fun(&click_handler));
	button->onMouseMove.connect(sigc::ptr_fun(&move_handler));

#if 0
	Gui::Image *image;
	screen->SetInnerWidget(
		c->Background(Color(0.4f, 0.2f, 0.4f, 1.0f))->SetInnerWidget(
			c->Margin(10.0f)->SetInnerWidget(
				c->Background(Color(0.1f, 0.4f, 0.4f, 1.0f))->SetInnerWidget(
					c->VBox()->PackEnd(Gui::WidgetSet(
						c->HBox()->PackEnd(Gui::WidgetSet(
							c->Label("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."),
							(image = c->Image("icons/object_star_g.png")),
							c->Image("icons/object_star_m.png")
						)),
						c->Background(Color(1.0f, 0.0f, 0.0f, 1.0f)),
						c->Background(Color(0.0f, 1.0f, 0.0f, 1.0f)),
						c->Background(Color(0.0f, 0.0f, 1.0f, 1.0f)),
						c->Image("icons/cpanel.png")
					))
				)
			)
		)
	);

	image->onMouseUp.connect(sigc::ptr_fun(&click_handler));
	image->onMouseMove.connect(sigc::ptr_fun(&move_handler));
#endif

	while (1) {
		bool done = false;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				done = true;
			else
				Gui::Event::DispatchSDLEvent(event, screen);
		}

		if (done)
			break;

		screen->Layout();
		screen->Update();

		r->ClearScreen();
		screen->Draw();
		r->SwapBuffers();
	}

	delete screen;
	delete c;
	delete r;

	SDL_Quit();

	exit(0);
}
