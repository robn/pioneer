#include "libs.h"
#include "Pi.h"
#include "Gui.h"
#include "glfreetype.h"
#include "Player.h"
#include "Space.h"
#include "Planet.h"
#include "Star.h"
#include "Frame.h"
#include "ShipCpanel.h"
#include "ShipType.h"
#include "Sector.h"
#include "SectorView.h"
#include "SystemView.h"
#include "SystemInfoView.h"
#include "WorldView.h"
#include "ObjectViewerView.h"
#include "StarSystem.h"
#include "SpaceStation.h"
#include "SpaceStationView.h"
#include "CargoBody.h"
#include "InfoView.h"
#include "Serializer.h"
#include "NameGenerator.h"
#include "GeoSphere.h"
#include "Sound.h"
#include "Polit.h"
#include "GalacticView.h"
#include "Galaxy.h"
#include "GameMenuView.h"
#include "Missile.h"
#include "LmrModel.h"
#include "Render.h"
#include "PiLuaModules.h"
#include "AmbientSounds.h"

float Pi::gameTickAlpha;
int Pi::timeAccelIdx = 1;
int Pi::requestedTimeAccelIdx = 1;
int Pi::scrWidth;
int Pi::scrHeight;
float Pi::scrAspect;
SDL_Surface *Pi::scrSurface;
sigc::signal<void, SDL_keysym*> Pi::onKeyPress;
sigc::signal<void, SDL_keysym*> Pi::onKeyRelease;
sigc::signal<void, int, int, int> Pi::onMouseButtonUp;
sigc::signal<void, int, int, int> Pi::onMouseButtonDown;
sigc::signal<void> Pi::onPlayerChangeTarget;
sigc::signal<void> Pi::onPlayerChangeHyperspaceTarget;
sigc::signal<void> Pi::onPlayerHyperspaceToNewSystem;
sigc::signal<void> Pi::onPlayerChangeFlightControlState;
sigc::signal<void> Pi::onPlayerChangeEquipment;
sigc::signal<void, const SpaceStation*> Pi::onDockingClearanceExpired;
int Pi::keyModState;
char Pi::keyState[SDLK_LAST];
char Pi::mouseButton[6];
int Pi::mouseMotion[2];
Player *Pi::player;
View *Pi::currentView;
WorldView *Pi::worldView;
ObjectViewerView *Pi::objectViewerView;
SpaceStationView *Pi::spaceStationView;
InfoView *Pi::infoView;
SectorView *Pi::sectorView;
GalacticView *Pi::galacticView;
GameMenuView *Pi::gameMenuView;
SystemView *Pi::systemView;
SystemInfoView *Pi::systemInfoView;
ShipCpanel *Pi::cpan;
StarSystem *Pi::selectedSystem;
StarSystem *Pi::currentSystem;
MTRand Pi::rng;
double Pi::gameTime;
float Pi::frameTime;
GLUquadric *Pi::gluQuadric;
bool Pi::showDebugInfo;
int Pi::statSceneTris;
bool Pi::isGameStarted = false;
IniConfig Pi::config;
struct DetailLevel Pi::detail = {};
std::vector<Pi::JoystickState> Pi::joysticks;
const float Pi::timeAccelRates[] = { 0.0, 1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0 };
const char * const Pi::combatRating[] = {
	"Harmless",
	"Mostly harmless",
	"Poor",
	"Average",
	"Above Average",
	"Competent",
	"Dangerous",
	"Deadly",
	"ELITE"
};

int Pi::CombatRating(int kills)
{
	if (kills < 8) return 0;
	if (kills < 16) return 1;
	if (kills < 32) return 2;
	if (kills < 64) return 3;
	if (kills < 128) return 4;
	if (kills < 512) return 5;
	if (kills < 2400) return 6;
	if (kills < 6000) return 7;
	/* nothing better to do with their lives? */
	return 8;
}

static void draw_progress(float progress)
{
	float w, h;
	Render::PrepareFrame();
	Render::PostProcess();
	Gui::Screen::EnterOrtho();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::string msg = stringf(256, "Simulating evolution of the universe: %.1f billion years ;-)", progress * 15.0f);
	Gui::Screen::MeasureString(msg, w, h);
	glColor3f(1.0f,1.0f,1.0f);
	Gui::Screen::RenderString(msg, 0.5f*(Gui::Screen::GetWidth()-w), 0.5f*(Gui::Screen::GetHeight()-h));
	Gui::Screen::LeaveOrtho();
	Render::SwapBuffers();
}

void Pi::Init()
{
	config.Load(GetPiUserDir() + "config.ini");

	Pi::detail.planets = config.Int("DetailPlanets");
	Pi::detail.cities = config.Int("DetailCities");

	int width = config.Int("ScrWidth");
	int height = config.Int("ScrHeight");
	const SDL_VideoInfo *info = NULL;
	Uint32 sdlInitFlags = SDL_INIT_VIDEO | SDL_INIT_JOYSTICK;
#if defined _WIN32 && defined _DEBUG
	sdlInitFlags |= SDL_INIT_NOPARACHUTE;
#endif
	if (SDL_Init(sdlInitFlags) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		exit(-1);
	}

	InitJoysticks();

	// no mode set, find an ok one
	if ((width <= 0) || (height <= 0)) {
		SDL_Rect **modes = SDL_ListModes(NULL, SDL_HWSURFACE | SDL_FULLSCREEN);
		
		if (modes == 0) {
			fprintf(stderr, "It seems no video modes are available...");
		}
		if (modes == (SDL_Rect **)-1) {
			// hm. all modes available. odd. try 800x600
			width = 800; height = 600;
		} else {
			width = modes[0]->w;
			height = modes[0]->h;
		}
	}

	info = SDL_GetVideoInfo();
	printf("SDL_GetVideoInfo says %d bpp\n", info->vfmt->BitsPerPixel);
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
			fprintf(stderr, "Invalid pixel depth: %d bpp\n", info->vfmt->BitsPerPixel);
	} 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Uint32 flags = SDL_OPENGL;
	if (config.Int("StartFullscreen")) flags |= SDL_FULLSCREEN;

	if ((Pi::scrSurface = SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, flags)) == 0) {
		// fall back on 16-bit depth buffer...
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		fprintf(stderr, "Failed to set video mode. (%s). Re-trying with 16-bit depth buffer.\n", SDL_GetError());
		if ((Pi::scrSurface = SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, flags)) == 0) {
			fprintf(stderr, "Failed to set video mode: %s", SDL_GetError());
		}
	}

	glewInit();
	SDL_WM_SetCaption("Pioneer","Pioneer");
	Pi::scrWidth = width;
	Pi::scrHeight = height;
	Pi::scrAspect = width / (float)height;

	Pi::rng.seed(time(NULL));

	InitOpenGL();
	GLFTInit();
	// Gui::Init shouldn't initialise any VBOs, since we haven't tested
	// that the capability exists. (Gui does not use VBOs so far)
	Gui::Init(scrWidth, scrHeight, 800, 600);
	if (!GLEW_ARB_vertex_buffer_object) {
		Error("OpenGL extension ARB_vertex_buffer_object not supported. Pioneer can not run on your graphics card.");
	}
	Render::Init(width, height);
	draw_progress(0.1f);
	Galaxy::Init();
	draw_progress(0.2f);
	NameGenerator::Init();
	draw_progress(0.3f);
	Sector::Init();
	if (config.Int("DisableShaders")) Render::ToggleShaders();
	if (config.Int("EnableHDR")) Render::ToggleHDR();

	draw_progress(0.4f);
	LmrModelCompilerInit();
	draw_progress(0.5f);
	ShipType::Init();
	draw_progress(0.6f);
	GeoSphere::Init();
	draw_progress(0.7f);
	Space::Init();
	draw_progress(0.8f);
	Polit::Init();
	draw_progress(0.9f);
	SpaceStation::Init();

	if (!config.Int("DisableSound")) {
		Sound::Init();
		Sound::SetGlobalVolume(config.Float("SfxVolume"));
		Sound::Pause(0);
	}
	draw_progress(1.0f);
	
	gameMenuView = new GameMenuView();
	config.Save();
}

void Pi::InitOpenGL()
{
	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glClearColor(0,0,0,0);
	glViewport(0, 0, scrWidth, scrHeight);

	gluQuadric = gluNewQuadric ();
	
	fprintf(stderr, "GL_ARB_point_sprite: %s\n", GLEW_ARB_point_sprite ? "Yes" : "No");
}

void Pi::Quit()
{
	SDL_Quit();
	exit(0);
}

void Pi::BoinkNoise()
{
	Sound::PlaySfx("Click", 0.3f, 0.3f, false);
}

void Pi::SetTimeAccel(int s)
{
	// don't want player to spin like mad when hitting time accel
	if ((s != timeAccelIdx) && (s > 2)) {
		player->SetAngVelocity(vector3d(0,0,0));
		player->SetTorque(vector3d(0,0,0));
		player->SetAngThrusterState(0, 0.0f);
		player->SetAngThrusterState(1, 0.0f);
		player->SetAngThrusterState(2, 0.0f);
	}
	timeAccelIdx = s;
}

void Pi::RequestTimeAccel(int s)
{
	if (currentView == gameMenuView) {
		SetView(worldView);
	}
	requestedTimeAccelIdx = s;
}

void Pi::SetView(View *v)
{
	if (currentView) currentView->HideAll();
	currentView = v;
	currentView->OnSwitchTo();
	currentView->ShowAll();
}

void Pi::OnChangeDetailLevel()
{
	GeoSphere::OnChangeDetailLevel();
}

void Screendump(char *destFile)
{
	/* XXX TODO XXX not endian-safe */
	const int W = Pi::GetScrWidth();
	const int H = Pi::GetScrHeight();
	std::vector<char> pixel_data(3*W*H);
	short TGAhead[] = {0, 2, 0, 0, 0, 0, W, H, 24};
	FILE *out = fopen(destFile, "w");
	if (!out) goto error;
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, W, H, GL_BGR, GL_UNSIGNED_BYTE, &pixel_data[0]);
	if (fwrite(&TGAhead, sizeof(TGAhead), 1, out) != 1) {
		fclose(out);
		goto error;
	}
	if (fwrite(&pixel_data[0], 3*W*H, 1, out) != 1) {
		fclose(out);
		goto error;
	}
	fclose(out);
	return;
error:
	printf("Failed to write screendump.\n");
}

void Pi::HandleEvents()
{
	SDL_Event event;

	Pi::mouseMotion[0] = Pi::mouseMotion[1] = 0;
	while (SDL_PollEvent(&event)) {
		Gui::HandleSDLEvent(&event);
		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					// only accessible once game started
					if (currentView != 0) {
						if (currentView != gameMenuView) {
							RequestTimeAccel(0);
							SetTimeAccel(0);
							SetView(gameMenuView);
						}
						else
							RequestTimeAccel(1);
					}
					break;
				}
				// special keys. LCTRL+turd
				if ((KeyState(SDLK_LCTRL) || (KeyState(SDLK_RCTRL)))) {
					if (event.key.keysym.sym == SDLK_q) Pi::Quit();
					if (event.key.keysym.sym == SDLK_s) {
						Render::ToggleShaders();
					}
					if (event.key.keysym.sym == SDLK_h) {
						Render::ToggleHDR();
					}
					if (event.key.keysym.sym == SDLK_i) Pi::showDebugInfo = !Pi::showDebugInfo;
					if (event.key.keysym.sym == SDLK_p) {
						Sint64 crime, fine;
						Polit::GetCrime(&crime, &fine);
						printf("Criminal record: %llx, $%lld\n", crime, fine);
						Polit::AddCrime(0x1, 100);
						Polit::GetCrime(&crime, &fine);
						printf("Criminal record now: %llx, $%lld\n", crime, fine);
					}
					if (event.key.keysym.sym == SDLK_PRINT) {
						char buf[256];
						const time_t t = time(0);
						struct tm *_tm = localtime(&t);
						strftime(buf, sizeof(buf), "screenshot-%Y%m%d-%H%M%S.tga", _tm);
						Screendump(buf);
						fprintf(stderr, "Screendump to %s\n", buf);
					}
#ifdef DEBUG
					if (event.key.keysym.sym == SDLK_m) {
						Pi::player->SetMoney(Pi::player->GetMoney() + 10000000);
					}
					if (event.key.keysym.sym == SDLK_F12) {
						matrix4x4d m; Pi::player->GetRotMatrix(m);
						vector3d dir = m*vector3d(0,0,-1);
						/* add test object */
						if (KeyState(SDLK_RSHIFT)) {
							Missile *missile = new Missile(ShipType::MISSILE_GUIDED, Pi::player, Pi::player->GetCombatTarget());
							missile->SetRotMatrix(m);
							missile->SetFrame(Pi::player->GetFrame());
							missile->SetPosition(Pi::player->GetPosition()+50.0*dir);
							missile->SetVelocity(Pi::player->GetVelocity());
							Space::AddBody(missile);
						} else if (KeyState(SDLK_LSHIFT)) {
							SpaceStation *s = static_cast<SpaceStation*>(Pi::player->GetNavTarget());
							if (s) {
								int port = s->GetFreeDockingPort();
								if (port != -1) {
									printf("Putting ship into station\n");
									// Make police ship intent on killing the player
									Ship *ship = new Ship(ShipType::LADYBIRD);
									ship->AIInstruct(Ship::DO_KILL, Pi::player);
									ship->SetFrame(Pi::player->GetFrame());
									ship->SetDockedWith(s, port);
									Space::AddBody(ship);
								} else {
									printf("No docking ports free dude\n");
								}
							} else {
								printf("Select a space station...\n");
							}
						} else {
							Ship *ship = new Ship(ShipType::LADYBIRD);
							ship->m_equipment.Set(Equip::SLOT_LASER, 0, Equip::PULSECANNON_1MW);
							ship->AIInstruct(Ship::DO_KILL, Pi::player);
							ship->SetFrame(Pi::player->GetFrame());
							ship->SetPosition(Pi::player->GetPosition()+100.0*dir);
							ship->SetVelocity(Pi::player->GetVelocity());
							ship->m_equipment.Add(Equip::DRIVE_CLASS2);
							ship->m_equipment.Add(Equip::RADAR_MAPPER);
							ship->m_equipment.Add(Equip::SCANNER);
							ship->m_equipment.Add(Equip::SHIELD_GENERATOR);
							ship->m_equipment.Add(Equip::HYDROGEN, 10);
							Space::AddBody(ship);
						}
					}
#endif /* DEBUG */
					// XXX only works on X11
					//if (event.key.keysym.sym == SDLK_F11) SDL_WM_ToggleFullScreen(Pi::scrSurface);
					if (event.key.keysym.sym == SDLK_F10) Pi::SetView(Pi::objectViewerView);
					if (event.key.keysym.sym == SDLK_F9) {
						std::string name = join_path(GetFullSavefileDirPath().c_str(), "_quicksave", 0);
						Serializer::SaveGame(name.c_str());
						Pi::cpan->MsgLog()->Message("", "Game saved to "+name);
					}
				}
				Pi::keyState[event.key.keysym.sym] = 1;
				Pi::keyModState = event.key.keysym.mod;
				Pi::onKeyPress.emit(&event.key.keysym);
				break;
			case SDL_KEYUP:
				Pi::keyState[event.key.keysym.sym] = 0;
				Pi::keyModState = event.key.keysym.mod;
				Pi::onKeyRelease.emit(&event.key.keysym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				Pi::mouseButton[event.button.button] = 1;
				Pi::onMouseButtonDown.emit(event.button.button,
						event.button.x, event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				Pi::mouseButton[event.button.button] = 0;
				Pi::onMouseButtonUp.emit(event.button.button,
						event.button.x, event.button.y);
				break;
			case SDL_MOUSEMOTION:
				Pi::mouseMotion[0] += event.motion.xrel;
				Pi::mouseMotion[1] += event.motion.yrel;
		//		SDL_GetRelativeMouseState(&Pi::mouseMotion[0], &Pi::mouseMotion[1]);
				break;
			case SDL_JOYAXISMOTION:
				if (joysticks[event.jaxis.which].joystick == NULL)
					break;
				if (event.jaxis.value == -32768)
					joysticks[event.jaxis.which].axes[event.jaxis.axis] = 1.f;
				else
					joysticks[event.jaxis.which].axes[event.jaxis.axis] = -event.jaxis.value / 32767.f;
				break;
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
				if (joysticks[event.jaxis.which].joystick == NULL)
					break;
				joysticks[event.jbutton.which].buttons[event.jbutton.button] = event.jbutton.state;
				break;
			case SDL_JOYHATMOTION:
				if (joysticks[event.jaxis.which].joystick == NULL)
					break;
				joysticks[event.jhat.which].hats[event.jhat.hat] = event.jhat.value;
				break;
			case SDL_QUIT:
				Pi::Quit();
				break;
		}
	}
}

static void draw_intro(WorldView *view, float _time)
{
	float lightCol[4] = { 1,1,1,0 };
	float lightDir[4] = { 0,1,1,0 };
	float ambient[4] = { 0.1,0.1,0.1,1 };

	// defaults are dandy
	Render::State::SetZnearZfar(1.0f, 10000.0f);
	LmrObjParams params = {
		{ },
		{ "PIONEER" },
		{ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 0.0f },
		{	// pColor[3]
		{ { .2f, .2f, .5f, 1.0f }, { 1, 1, 1 }, { 0, 0, 0 }, 100.0 },
		{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 },
		{ { 0.8f, 0.8f, 0.8f, 1.0f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 } },
	};
	glPushMatrix();
	glRotatef(_time*10, 1, 0, 0);
	view->DrawBgStars();
	glPopMatrix();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightCol);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightCol);
	glEnable(GL_LIGHT0);
	
	matrix4x4f rot = matrix4x4f::RotateYMatrix(_time) * matrix4x4f::RotateZMatrix(0.6*_time) *
			matrix4x4f::RotateXMatrix(_time*.7);
	rot[14] = -80.0;
	LmrLookupModelByName("lanner_ub")->Render(rot, &params);
	Render::State::UseProgram(0);
	Render::UnbindAllBuffers();
	glPopAttrib();
}

static void draw_tombstone(float _time)
{
	float lightCol[4] = { 1,1,1,0 };
	float lightDir[4] = { 0,1,1,0 };
	float ambient[4] = { 0.1,0.1,0.1,1 };

	LmrObjParams params = {
		{ 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ "RIP OLD BEAN" },
		{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f },
		{	// pColor[3]
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 },
		{ { 0.8f, 0.6f, 0.5f, 1.0f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 },
		{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 } },
	};
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightCol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightCol);
	glEnable(GL_LIGHT0);
	
	matrix4x4f rot = matrix4x4f::RotateYMatrix(_time*2);
	rot[14] = -MAX(150 - 30*_time, 30);
	LmrLookupModelByName("tombstone")->Render(rot, &params);
	Render::State::UseProgram(0);
	Render::UnbindAllBuffers();
	glPopAttrib();
}

void Pi::TombStoneLoop()
{
	Render::State::SetZnearZfar(1.0f, 10000.0f);

	Uint32 last_time = SDL_GetTicks();
	float _time = 0;
	cpan->HideAll();
	currentView->HideAll();
	do {
		Render::PrepareFrame();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float fracH = 1.0 / Pi::GetScrAspect();
		glFrustum(-1, 1, -fracH, fracH, 1.0f, 10000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Pi::HandleEvents();
		SDL_ShowCursor(1);
		SDL_WM_GrabInput(SDL_GRAB_OFF);

		draw_tombstone(_time);
		Render::PostProcess();
		Gui::Draw();
		Render::SwapBuffers();
		
		Pi::frameTime = 0.001*(SDL_GetTicks() - last_time);
		_time += Pi::frameTime;
		last_time = SDL_GetTicks();
	} while (!((_time > 2.0) && ((Pi::MouseButtonState(1)) || Pi::KeyState(SDLK_SPACE)) ));
}

void Pi::InitGame()
{
	// this is a bit brittle. skank may be forgotten and survive between
	// games
	Pi::timeAccelIdx = 1;
	Pi::requestedTimeAccelIdx = 1;
	Pi::gameTime = 0;
	Pi::currentView = 0;
	Pi::isGameStarted = false;

	player = new Player("Eagle Long Range Fighter");
	player->m_equipment.Set(Equip::SLOT_ENGINE, 0, Equip::DRIVE_CLASS1);
	player->m_equipment.Set(Equip::SLOT_LASER, 0, Equip::PULSECANNON_2MW);
	player->m_equipment.Add(Equip::HYDROGEN, 10);
	player->m_equipment.Add(Equip::ATMOSPHERIC_SHIELDING);
	player->m_equipment.Add(Equip::MISSILE_UNGUIDED);
	player->m_equipment.Add(Equip::MISSILE_UNGUIDED);
	player->m_equipment.Add(Equip::MISSILE_GUIDED);
	player->m_equipment.Add(Equip::MISSILE_GUIDED);
	player->m_equipment.Add(Equip::MISSILE_SMART);
	player->m_equipment.Add(Equip::MISSILE_NAVAL);
	player->m_equipment.Add(Equip::AUTOPILOT);
	player->m_equipment.Add(Equip::SCANNER);
	player->SetMoney(10000);
	Space::AddBody(player);
	
	cpan = new ShipCpanel();
	worldView = new WorldView();
	sectorView = new SectorView();
	galacticView = new GalacticView();
	systemView = new SystemView();
	systemInfoView = new SystemInfoView();
	objectViewerView = new ObjectViewerView();
	spaceStationView = new SpaceStationView();
	infoView = new InfoView();
	AmbientSounds::Init();
	PiLuaModules::Init();
}

static void OnPlayerDockOrUndock()
{
	Pi::RequestTimeAccel(1);
	Pi::SetTimeAccel(1);
}

static void OnPlayerChangeEquipment()
{
	Pi::onPlayerChangeEquipment.emit();
}

void Pi::StartGame()
{
	Pi::player->onDock.connect(sigc::ptr_fun(&OnPlayerDockOrUndock));
	Pi::player->onUndock.connect(sigc::ptr_fun(&OnPlayerDockOrUndock));
	Pi::player->m_equipment.onChange.connect(sigc::ptr_fun(&OnPlayerChangeEquipment));
	cpan->ShowAll();
	OnPlayerChangeEquipment();
	SetView(worldView);
	Pi::isGameStarted = true;
}

void Pi::UninitGame()
{
	AmbientSounds::Uninit();
	Sound::DestroyAllEvents();
	PiLuaModules::Uninit();
	Pi::isGameStarted = false;
	delete infoView;
	delete spaceStationView;
	delete objectViewerView;
	delete worldView;
	delete systemInfoView;
	delete systemView;
	delete sectorView;
	delete cpan;
	delete galacticView;
	if (Pi::player) {
		Space::KillBody(Pi::player);
		Space::RemoveBody(Pi::player);
		Space::Clear();
		delete Pi::player;
		Pi::player = 0;
	}
}

void Pi::Start()
{
	WorldView *view = new WorldView();
	
	Gui::Fixed *splash = new Gui::Fixed(Gui::Screen::GetWidth(), Gui::Screen::GetHeight());
	Gui::Screen::AddBaseWidget(splash, 0, 0);
	splash->SetTransparency(true);

	const float w = Gui::Screen::GetWidth() / 2;
	const float h = Gui::Screen::GetHeight() / 2;
	const int OPTS = 5;
	Gui::ToggleButton *opts[OPTS];
	opts[0] = new Gui::ToggleButton(); opts[0]->SetShortcut(SDLK_1, KMOD_NONE);
	opts[1] = new Gui::ToggleButton(); opts[1]->SetShortcut(SDLK_2, KMOD_NONE);
	opts[2] = new Gui::ToggleButton(); opts[2]->SetShortcut(SDLK_3, KMOD_NONE);
	opts[3] = new Gui::ToggleButton(); opts[3]->SetShortcut(SDLK_4, KMOD_NONE);
	opts[4] = new Gui::ToggleButton(); opts[4]->SetShortcut(SDLK_5, KMOD_NONE);
	splash->Add(opts[0], w, h-64);
	splash->Add(new Gui::Label("New game starting on Earth"), w+32, h-64);
	splash->Add(opts[1], w, h-32);
	splash->Add(new Gui::Label("New game starting on Epsilon Eridani"), w+32, h-32);
	splash->Add(opts[2], w, h);
	splash->Add(new Gui::Label("New game starting on debug point"), w+32, h);
	splash->Add(opts[3], w, h+32);
	splash->Add(new Gui::Label("Load a saved game"), w+32, h+32);
	splash->Add(opts[4], w, h+64);
	splash->Add(new Gui::Label("Quit"), w+32, h+64);

	splash->ShowAll();

	int choice = 0;
	Uint32 last_time = SDL_GetTicks();
	float _time = 0;
	do {
		Pi::HandleEvents();

		Render::PrepareFrame();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float fracH = 1.0 / Pi::GetScrAspect();
		glFrustum(-1, 1, -fracH, fracH, 1.0f, 10000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_ShowCursor(1);
		SDL_WM_GrabInput(SDL_GRAB_OFF);

		draw_intro(view, _time);
		Render::PostProcess();
		Gui::Draw();
		Render::SwapBuffers();
		
		Pi::frameTime = 0.001*(SDL_GetTicks() - last_time);
		_time += Pi::frameTime;
		last_time = SDL_GetTicks();

		// poll ui instead of using callbacks :-J
		for (int i=0; i<OPTS; i++) if (opts[i]->GetPressed()) choice = i+1;
	} while (!choice);
	splash->HideAll();
	
	Gui::Screen::RemoveBaseWidget(splash);
	delete splash;
	delete view;
	
	InitGame();

	if (choice == 1) {
		/* Earth start point */
		SBodyPath path(0,0,0);
		Space::DoHyperspaceTo(&path);
		//Frame *pframe = *(++(++(Space::rootFrame->m_children.begin())));
		//player->SetFrame(pframe);
		// XXX there isn't a sensible way to find stations for a planet.
		SpaceStation *station = 0;
		for (Space::bodiesIter_t i = Space::bodies.begin(); i!=Space::bodies.end(); i++) {
			if ((*i)->IsType(Object::SPACESTATION)) { station = (SpaceStation*)*i; break; }
		}
		assert(station);
		player->SetPosition(vector3d(0,0,0));
		player->SetFrame(station->GetFrame());
		player->SetDockedWith(station, 0);
		MainLoop();
	} else if (choice == 2) {
		/* Epsilon Eridani start point */
		SBodyPath path(1,0,2);
		Space::DoHyperspaceTo(&path);
		// XXX there isn't a sensible way to find stations for a planet.
		SpaceStation *station = 0;
		for (Space::bodiesIter_t i = Space::bodies.begin(); i!=Space::bodies.end(); i++) {
			if ((*i)->IsType(Object::SPACESTATION)) {
				station = (SpaceStation*)*i;
				if (!station->IsGroundStation()) break;
			}
		}
		assert(station);
		player->SetPosition(vector3d(0,0,0));
		player->SetFrame(station->GetFrame());
		player->SetDockedWith(station, 0);
		MainLoop();
	} else if (choice == 3) {
		/* debug start point */
		SBodyPath path(1,0,2);
		path.sbodyId = 6;
		Space::DoHyperspaceTo(&path);
		player->SetPosition(vector3d(2*EARTH_RADIUS,0,0));
		player->m_equipment.Add(Equip::HYPERCLOUD_ANALYZER);
	/*	Frame *stationFrame = new Frame(pframe, "Station frame...");
		stationFrame->SetRadius(5000);
		stationFrame->m_sbody = 0;
		stationFrame->SetPosition(vector3d(0,0,zpos));
		stationFrame->SetAngVelocity(vector3d(0,0,0.5));

		for (int i=0; i<4; i++) {
			Ship *body = new Ship(ShipType::LADYBIRD);
			char buf[64];
			snprintf(buf,sizeof(buf),"X%c-0%02d", 'A'+i, i);
			body->SetLabel(buf);
			body->SetFrame(stationFrame);
			body->SetPosition(vector3d(200*(i+1), 0, 2000));
			Space::AddBody(body);
		}
			
		SpaceStation *station = new SpaceStation(SpaceStation::JJHOOP);
		station->SetLabel("Poemi-chan's Folly");
		station->SetFrame(stationFrame);
		station->SetPosition(vector3d(0,0,0));
		Space::AddBody(station);

		SpaceStation *station2 = new SpaceStation(SpaceStation::GROUND_FLAVOURED);
		station2->SetLabel("Conor's End");
		station2->SetFrame(*pframe->m_children.begin()); // rotating frame of planet
		station2->OrientOnSurface(EARTH_RADIUS, M_PI/4, M_PI/4);
		Space::AddBody(station2);
	*/
	//	player->SetDockedWith(station2, 0);

		MainLoop();
	} else if (choice == 4) {
		if (Pi::player) {
			Pi::player->MarkDead();
			Space::bodies.remove(Pi::player);
			delete Pi::player;
			Pi::player = 0;
		}
		Pi::gameMenuView->OpenLoadDialog();
		do {
			Gui::MainLoopIteration();
		} while (Pi::currentView != Pi::worldView);
		
		if (Pi::isGameStarted) MainLoop();
	} else {
		Pi::Quit();
	}
	UninitGame();
}

void Pi::EndGame()
{
	Pi::isGameStarted = false;
}


void Pi::MainLoop()
{
	StartGame();
	
	Uint32 last_stats = SDL_GetTicks();
	int frame_stat = 0;
	int phys_stat = 0;
	char fps_readout[128];
	double time_player_died = 0;
#ifdef MAKING_VIDEO
	Uint32 last_screendump = SDL_GetTicks();
	int dumpnum = 0;
#endif /* MAKING_VIDEO */

	double currentTime = 0.001 * (double)SDL_GetTicks();
	double accumulator = Pi::GetTimeStep();
	Pi::gameTickAlpha = 0;

	memset(fps_readout, 0, sizeof(fps_readout));

	while (isGameStarted) {
		double newTime = 0.001 * (double)SDL_GetTicks();
		Pi::frameTime = newTime - currentTime;
		if (Pi::frameTime > 0.25) Pi::frameTime = 0.25;
		currentTime = newTime;
		accumulator += Pi::frameTime * GetTimeAccel();
		
		const float step = Pi::GetTimeStep();
		if (step) {
			while (accumulator >= step) {
				Space::TimeStep(step);
				gameTime += step;
				phys_stat++;

				accumulator -= step;
			}
			Pi::gameTickAlpha = accumulator / step;
		} else {
			// paused
		}

		if (frame_stat == 0) {
			// called not more than once per second
			PiLuaModules::UpdateOncePerRealtimeSecond();
		}
		frame_stat++;

		Render::PrepareFrame();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		/* Calculate position for this rendered frame (interpolated between two physics ticks */
		for (std::list<Body*>::iterator i = Space::bodies.begin(); i != Space::bodies.end(); ++i) {
			(*i)->UpdateInterpolatedTransform(Pi::GetGameTickAlpha());
		}

		currentView->Draw3D();
		// XXX HandleEvents at the moment must be after view->Draw3D and before
		// Gui::Draw so that labels drawn to screen can have mouse events correctly
		// detected. Gui::Draw wipes memory of label positions.
		Pi::HandleEvents();
		// hide cursor for ship control.
		if (Pi::MouseButtonState(3)) {
			SDL_ShowCursor(0);
			SDL_WM_GrabInput(SDL_GRAB_ON);
		} else {
			SDL_ShowCursor(1);
			SDL_WM_GrabInput(SDL_GRAB_OFF);
		}

		Render::PostProcess();
		Gui::Draw();
//#ifdef DEBUG
		if (Pi::showDebugInfo) {
			Gui::Screen::EnterOrtho();
			glColor3f(1,1,1);
			Gui::Screen::RenderString(fps_readout, 0, 0);
			Gui::Screen::LeaveOrtho();
		}
//#endif /* DEBUG */

		glError();
		Render::SwapBuffers();
		//if (glGetError()) printf ("GL: %s\n", gluErrorString (glGetError ()));
		
		int timeAccel = Pi::requestedTimeAccelIdx;
		if (Pi::player->GetFlightState() == Ship::FLYING) {
			// check we aren't too near to objects for timeaccel //
			for (std::list<Body*>::iterator i = Space::bodies.begin(); i != Space::bodies.end(); ++i) {
				if ((*i) == Pi::player) continue;
				if ((*i)->IsType(Object::HYPERSPACECLOUD)) continue;
				
				vector3d toBody = Pi::player->GetPosition() - (*i)->GetPositionRelTo(Pi::player->GetFrame());
				double dist = toBody.Length();
				double rad = (*i)->GetBoundingRadius();

				if (dist < 1000.0) {
					timeAccel = MIN(timeAccel, 1);
				} else if (dist < MIN(rad+0.0001*AU, rad*1.1)) {
					timeAccel = MIN(timeAccel, 2);
				} else if (dist < MIN(rad+0.001*AU, rad*5.0)) {
					timeAccel = MIN(timeAccel, 3);
				} else if (dist < MIN(rad+0.01*AU,rad*10.0)) {
					timeAccel = MIN(timeAccel, 4);
				} else if (dist < MIN(rad+0.1*AU, rad*1000.0)) {
					timeAccel = MIN(timeAccel, 5);
				}
			}
		}
		Pi::SetTimeAccel(timeAccel);

		// fuckadoodledoo, did the player die?
		if (Pi::player->IsDead()) {
			if (time_player_died) {
				if (Pi::GetGameTime() - time_player_died > 8.0) {
					Sound::DestroyAllEvents();
					Pi::TombStoneLoop();
					break;
				}
			} else {
				Pi::SetTimeAccel(1);
				Pi::cpan->HideAll();
				Pi::SetView(static_cast<View*>(Pi::worldView));
				Pi::player->Disable();
				time_player_died = Pi::GetGameTime();
			}
		} else {
			// this is something we need not do every turn...
			AmbientSounds::Update();
			StarSystem::ShrinkCache();
		}
		cpan->Update();
		currentView->Update();

		if (SDL_GetTicks() - last_stats > 1000) {
			Pi::statSceneTris += LmrModelGetStatsTris();
			snprintf(fps_readout, sizeof(fps_readout), "%d fps, %d phys updates, %d triangles, %.3f M tris/sec", frame_stat, phys_stat, Pi::statSceneTris, Pi::statSceneTris*frame_stat*1e-6);
			frame_stat = 0;
			phys_stat = 0;
			last_stats += 1000;
		}
		Pi::statSceneTris = 0;
		LmrModelClearStatsTris();

#ifdef MAKING_VIDEO
		if (SDL_GetTicks() - last_screendump > 50) {
			last_screendump = SDL_GetTicks();
			char buf[256];
			snprintf(buf, sizeof(buf), "screenshot%08d.tga", dumpnum++);
			Screendump(buf);
		}
#endif /* MAKING_VIDEO */
	}
}

StarSystem *Pi::GetSelectedSystem()
{
	int sector_x, sector_y, system_idx;
	Pi::sectorView->GetSelectedSystem(&sector_x, &sector_y, &system_idx);
	if (system_idx == -1) {
		selectedSystem = 0;
		return NULL;
	}
	if (selectedSystem) {
		if (!selectedSystem->IsSystem(sector_x, sector_y, system_idx)) {
			delete selectedSystem;
			selectedSystem = 0;
		}
	}
	if (!selectedSystem) {
		selectedSystem = new StarSystem(sector_x, sector_y, system_idx);
	}
	return selectedSystem;
}

void Pi::Serialize(Serializer::Writer &wr)
{
	Serializer::Writer section;

	Serializer::IndexFrames();
	Serializer::IndexBodies();
	Serializer::IndexSystemBodies(currentSystem);

	section = Serializer::Writer();
	section.Double(gameTime);
	StarSystem::Serialize(section, selectedSystem);
	StarSystem::Serialize(section, currentSystem);
	wr.WrSection("PiMisc", section.GetData());
	
	section = Serializer::Writer();
	Space::Serialize(section);
	wr.WrSection("Space", section.GetData());

	section = Serializer::Writer();
	Polit::Serialize(section);
	wr.WrSection("Polit", section.GetData());
	
	section = Serializer::Writer();
	sectorView->Save(section);
	wr.WrSection("SectorView", section.GetData());

	section = Serializer::Writer();
	worldView->Save(section);
	wr.WrSection("WorldView", section.GetData());

	section = Serializer::Writer();
	PiLuaModules::Serialize(section);
	wr.WrSection("LuaModules", section.GetData());
}

void Pi::Unserialize(Serializer::Reader &rd)
{
	Serializer::Reader section;
	
	SetTimeAccel(0);
	requestedTimeAccelIdx = 0;
	Space::Clear();
	if (Pi::player) {
		Pi::player->MarkDead();
		Space::bodies.remove(Pi::player);
		delete Pi::player;
		Pi::player = 0;
	}

	section = rd.RdSection("PiMisc");
	gameTime = section.Double();
	selectedSystem = StarSystem::Unserialize(section);
	currentSystem = StarSystem::Unserialize(section);

	section = rd.RdSection("Space");
	Space::Unserialize(section);
	
	section = rd.RdSection("Polit");
	Polit::Unserialize(section);

	section = rd.RdSection("SectorView");
	sectorView->Load(section);

	section = rd.RdSection("WorldView");
	worldView->Load(section);

	section = rd.RdSection("LuaModules");
	PiLuaModules::Unserialize(section);
}

float Pi::CalcHyperspaceRange(int hyperclass, int total_mass_in_tonnes)
{
	return 200.0f * hyperclass * hyperclass / (float)total_mass_in_tonnes;
}

void Pi::Message(const std::string &message, const std::string &from, enum MsgLevel level)
{
	if (level == MSG_IMPORTANT) {
		Pi::cpan->MsgLog()->ImportantMessage(from, message);
	} else {
		Pi::cpan->MsgLog()->Message(from, message);
	}
}

void Pi::InitJoysticks() {
	int joy_count = SDL_NumJoysticks();
	for (int n = 0; n < joy_count; n++) {
		JoystickState *state;
		joysticks.push_back(JoystickState());
		state = &joysticks.back();

		state->joystick = SDL_JoystickOpen(n);
		if (state->joystick == NULL) {
			fprintf(stderr, "SDL_JoystickOpen(%i): %s\n", n, SDL_GetError());
			continue;
		}

		state->axes.resize(SDL_JoystickNumAxes(state->joystick));
		state->buttons.resize(SDL_JoystickNumButtons(state->joystick));
		state->hats.resize(SDL_JoystickNumHats(state->joystick));
	}
}

int Pi::JoystickButtonState(int joystick, int button) {
	if (joystick < 0 || joystick >= (int) joysticks.size())
		return 0;

	if (button < 0 || button >= (int) joysticks[joystick].buttons.size())
		return 0;

	return joysticks[joystick].buttons[button];
}

int Pi::JoystickHatState(int joystick, int hat) {
	if (joystick < 0 || joystick >= (int) joysticks.size())
		return 0;

	if (hat < 0 || hat >= (int) joysticks[joystick].hats.size())
		return 0;

	return joysticks[joystick].hats[hat];
}

float Pi::JoystickAxisState(int joystick, int axis) {
	if (joystick < 0 || joystick >= (int) joysticks.size())
		return 0;

	if (axis < 0 || axis >= (int) joysticks[joystick].axes.size())
		return 0;

	return joysticks[joystick].axes[axis];
}
