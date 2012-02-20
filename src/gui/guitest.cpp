#include <cstdlib>
#include "SDL.h"
#include "GuiContext.h"
#include "GuiScreen.h"
#include "GuiBackground.h"
#include "GuiBox.h"
#include "GuiImage.h"
#include "GuiLabel.h"

static const int WIDTH  = 1024;
static const int HEIGHT = 768;

int main(int argc, char **argv)
{
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

	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0,0,0,1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Gui::Context *context = new Gui::Context;
	Gui::Screen *screen = new Gui::Screen(context, WIDTH, HEIGHT);

	Gui::Background *background = new Gui::Background(Color(0.4f, 0.2f, 0.4f, 1.0f));
	screen->SetInnerWidget(background);

	Gui::HBox *hbox = new Gui::HBox();
	background->SetInnerWidget(hbox);

	hbox->PackEnd(new Gui::Label("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));
	//box->PackEnd(new Gui::Label("foo   bar"));

	//hbox->PackEnd(new Gui::Image("data/icons/object_star_g.png"), Gui::Box::ChildAttrs(true, false));
	hbox->PackEnd(new Gui::Image("data/icons/object_star_m.png"));

	/*
	box->PackEnd(new Gui::Background(Color(1.0f, 0.0f, 0.0f, 1.0f)));
	box->PackEnd(new Gui::Background(Color(0.0f, 1.0f, 0.0f, 1.0f)));
	box->PackEnd(new Gui::Background(Color(0.0f, 0.0f, 1.0f, 1.0f)));
	box->PackEnd(new Gui::Image("data/icons/cpanel.png"));

	Gui::HBox *hbox = new Gui::HBox();
	box->PackEnd(hbox);
	hbox->PackEnd(new Gui::Background(Color(1.0f, 1.0f, 0.0f, 1.0f)));
	hbox->PackEnd(new Gui::Image("data/icons/cpanel.png"));
	*/

	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				done = true;
		}

		screen->Layout();
		screen->Update();

		glClear(GL_COLOR_BUFFER_BIT);
		screen->Draw();
		SDL_GL_SwapBuffers();
	}

	delete screen;
	delete context;

	SDL_Quit();

	exit(0);
}
