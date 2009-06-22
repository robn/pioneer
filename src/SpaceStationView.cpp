#include "SpaceStation.h"
#include "SpaceStationView.h"
#include "Pi.h"
#include "Player.h"
#include "WorldView.h"
#include "ShipFlavour.h"
#include <map>

#define TEXSIZE	128
#define ADD_VIDEO_WIDGET	Add(new DeadVideoLink(295,285), 5, 40)

class DeadVideoLink: public Gui::Widget {
public:
	void PutRandomCrapIntoTexture() {
		int *randcrap = (int*)alloca(TEXSIZE*TEXSIZE);
		for (unsigned int i=0; i<TEXSIZE*TEXSIZE/sizeof(int); i++) randcrap[i] = (Pi::rng.Int32() & 0xfcfcfcfc) >> 2;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, TEXSIZE, TEXSIZE, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, randcrap);
	}
	DeadVideoLink(float w, float h) {
		m_w = w; m_h = h;
		m_created = SDL_GetTicks();
		m_message = new Gui::ToolTip("Video link down");
		glEnable (GL_TEXTURE_2D);
		glGenTextures (1, &m_tex);
		glBindTexture (GL_TEXTURE_2D, m_tex);
		PutRandomCrapIntoTexture();
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glDisable (GL_TEXTURE_2D);
	}
	virtual ~DeadVideoLink() {
		glDeleteTextures(1, &m_tex);
		delete m_message;
	}
	virtual void Draw() {
		float size[2]; GetSize(size);
		if (SDL_GetTicks() - m_created < 1500) {
			m_message->SetText("Connecting...");
			glBegin(GL_QUADS);
				glColor3f(0,0,0);
				glVertex2f(0,0);
				glVertex2f(0,size[1]);
				glVertex2f(size[0],size[1]);
				glVertex2f(size[0],0);
			glEnd();
			DrawMessage();
		} else {
			m_message->SetText("Video link down");

			glEnable (GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_tex);
			PutRandomCrapIntoTexture();
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBegin(GL_QUADS);
				glColor3f(0,0,0);
				glTexCoord2f(0,0);
				glVertex2f(0,0);
				glTexCoord2f(0,1);
				glVertex2f(0,size[1]);
				glTexCoord2f(1,1);
				glVertex2f(size[0],size[1]);
				glTexCoord2f(1,0);
				glVertex2f(size[0],0);
			glEnd();
			glDisable (GL_TEXTURE_2D);
			if (SDL_GetTicks() & 0x400) {
				DrawMessage();
			}
		}
	}
	virtual void GetSizeRequested(float size[2]) {
		size[0] = m_w;
		size[1] = m_h;
	}
private:
	void DrawMessage() {
		float size[2];
		float msgSize[2];
		GetSize(size);
		m_message->GetSize(msgSize);
		glPushMatrix();
		glTranslatef(size[0]*0.5f-msgSize[0]*0.5f, size[1]*0.5f-msgSize[1]*0.5f, 0);
		m_message->Draw();
		glPopMatrix();
	}
	Uint32 m_created;
	GLuint m_tex;
	float m_w, m_h;
	Gui::ToolTip *m_message;
};

class StationSubView: public Gui::Fixed {
public:
	StationSubView(): Gui::Fixed((float)Gui::Screen::GetWidth(), (float)(Gui::Screen::GetHeight()-64)) {
	}
	void GoBack() {
		GetParent()->RemoveChild(this);
		GetParent()->ShowChildren();
		GetParent()->SetTransparency(false);
		delete this;
	}
};

////////////////////////////////////////////////////////////////////

class StationCommoditiesView: public StationSubView {
public:
	StationCommoditiesView();
	virtual void ShowAll();
private:
	void OnClickBuy(int commodity_type) {
		m_station->SellItemTo(Pi::player, (Equip::Type)commodity_type);
		UpdateStock(commodity_type);
		UpdateStats();
	}
	void OnClickSell(int commodity_type) {
		Pi::player->SellItemTo(m_station, (Equip::Type)commodity_type);
		UpdateStock(commodity_type);
		UpdateStats();
	}
	void UpdateStock(int commodity_type) {
		char buf[128];
		snprintf(buf, sizeof(buf), "%dt", Pi::player->m_equipment.Count(Equip::SLOT_CARGO, static_cast<Equip::Type>(commodity_type))*EquipType::types[commodity_type].mass);
		m_cargoLabels[commodity_type]->SetText(buf);
		
		snprintf(buf, sizeof(buf), "%dt", m_station->GetEquipmentStock(static_cast<Equip::Type>(commodity_type))*EquipType::types[commodity_type].mass);
		m_stockLabels[commodity_type]->SetText(buf);
	}
	void UpdateStats() {
		char buf[128];
		snprintf(buf, sizeof(buf), "Credits: $%lld", Pi::player->GetMoney());
		m_money->SetText(buf);
	}
	std::map<int, Gui::Label*> m_stockLabels;
	std::map<int, Gui::Label*> m_cargoLabels;
	Gui::Label *m_money;
	SpaceStation *m_station;
};

StationCommoditiesView::StationCommoditiesView(): StationSubView()
{
	SetTransparency(false);
}

void StationCommoditiesView::ShowAll()
{
	DeleteAllChildren();
	m_stockLabels.clear();
	m_cargoLabels.clear();

	m_station = Pi::player->GetDockedWith();
	assert(m_station);
	SetTransparency(false);
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "Welcome to %s commodities market", m_station->GetLabel().c_str());
		Add(new Gui::Label(buf), 10, 10);
	}

	Gui::Button *backButton = new Gui::SolidButton();
	backButton->onClick.connect(sigc::mem_fun(this, &StationCommoditiesView::GoBack));
	Add(backButton,680,470);
	Add(new Gui::Label("Go back"), 700, 470);

	m_money = new Gui::Label("");
	Add(m_money, 10, 450);
	
	Gui::Fixed *fbox = new Gui::Fixed(470, 400);
	Add(fbox, 320, 40);

	Gui::VScrollBar *scroll = new Gui::VScrollBar();
	Gui::VScrollPortal *portal = new Gui::VScrollPortal(450,400);
	scroll->SetAdjustment(&portal->vscrollAdjust);
	//int GetEquipmentStock(Equip::Type t) const { return m_equipmentStock[t]; }

	int NUM_ITEMS = 0;
	const float YSEP = Gui::Screen::GetFontHeight() * 1.5f;
	for (int i=1; i<Equip::TYPE_MAX; i++) {
		if (EquipType::types[i].slot == Equip::SLOT_CARGO) NUM_ITEMS++;
	}

	Gui::Fixed *innerbox = new Gui::Fixed(450, NUM_ITEMS*YSEP);
	for (int i=1, num=0; i<Equip::TYPE_MAX; i++) {
		if (EquipType::types[i].slot != Equip::SLOT_CARGO) continue;
		int stock = m_station->GetEquipmentStock(static_cast<Equip::Type>(i));
		Gui::Label *l = new Gui::Label(EquipType::types[i].name);
		if (EquipType::types[i].description)
			l->SetToolTip(EquipType::types[i].description);
		innerbox->Add(l,0,num*YSEP);
		Gui::Button *b = new Gui::SolidButton();
		b->onClick.connect(sigc::bind(sigc::mem_fun(this, &StationCommoditiesView::OnClickBuy), i));
		innerbox->Add(b, 380, num*YSEP);
		b = new Gui::SolidButton();
		b->onClick.connect(sigc::bind(sigc::mem_fun(this, &StationCommoditiesView::OnClickSell), i));
		innerbox->Add(b, 415, num*YSEP);
		char buf[128];
		snprintf(buf, sizeof(buf), "$%d", m_station->GetPrice(static_cast<Equip::Type>(i)));
		innerbox->Add(new Gui::Label(buf), 200, num*YSEP);
		
		snprintf(buf, sizeof(buf), "%dt", stock*EquipType::types[i].mass);
		Gui::Label *stocklabel = new Gui::Label(buf);
		m_stockLabels[i] = stocklabel;
		innerbox->Add(stocklabel, 275, num*YSEP);
		
		snprintf(buf, sizeof(buf), "%dt", Pi::player->m_equipment.Count(Equip::SLOT_CARGO, static_cast<Equip::Type>(i))*EquipType::types[i].mass);
		Gui::Label *cargolabel = new Gui::Label(buf);
		m_cargoLabels[i] = cargolabel;
		innerbox->Add(cargolabel, 325, num*YSEP);
		num++;
	}
	innerbox->ShowAll();

	fbox->Add(new Gui::Label("Item"), 0, 0);
	fbox->Add(new Gui::Label("Price"), 200, 0);
	fbox->Add(new Gui::Label("Buy"), 380, 0);
	fbox->Add(new Gui::Label("Sell"), 415, 0);
	fbox->Add(new Gui::Label("Stock"), 275, 0);
	fbox->Add(new Gui::Label("Cargo"), 325, 0);
	fbox->Add(portal, 0, YSEP);
	fbox->Add(scroll, 455, YSEP);
	portal->Add(innerbox);
	portal->ShowAll();
	fbox->ShowAll();
	UpdateStats();
	ADD_VIDEO_WIDGET;

	Gui::Fixed::ShowAll();
}

////////////////////////////////////////////////////////////////////

class StationShipUpgradesView: public StationSubView {
public:
	StationShipUpgradesView();
	virtual void ShowAll();
private:
};

StationShipUpgradesView::StationShipUpgradesView(): StationSubView()
{
	SetTransparency(false);
}

void StationShipUpgradesView::ShowAll()
{
	DeleteAllChildren();

	SpaceStation *station = Pi::player->GetDockedWith();
	assert(station);
	SetTransparency(false);
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "Welcome to %s shipyard", station->GetLabel().c_str());
		Add(new Gui::Label(buf), 10, 10);
	}
	
	Gui::Button *backButton = new Gui::SolidButton();
	backButton->onClick.connect(sigc::mem_fun(this, &StationShipUpgradesView::GoBack));
	Add(backButton,680,470);
	Add(new Gui::Label("Go back"), 700, 470);

	Gui::Fixed *fbox = new Gui::Fixed(470, 200);
	Add(fbox, 320, 40);

	Gui::VScrollBar *scroll = new Gui::VScrollBar();
	Gui::VScrollPortal *portal = new Gui::VScrollPortal(450,200);
	scroll->SetAdjustment(&portal->vscrollAdjust);
	//int GetEquipmentStock(Equip::Type t) const { return m_equipmentStock[t]; }

	int NUM_ITEMS = 0;
	const float YSEP = Gui::Screen::GetFontHeight() * 1.5f;
	for (int i=1; i<Equip::TYPE_MAX; i++) {
		if ((!(EquipType::types[i].slot == Equip::SLOT_CARGO)) &&
		    station->GetEquipmentStock(static_cast<Equip::Type>(i))) NUM_ITEMS++;
	}

	Gui::Fixed *innerbox = new Gui::Fixed(400, NUM_ITEMS*YSEP);
	for (int i=1, num=0; i<Equip::TYPE_MAX; i++) {
		if (EquipType::types[i].slot == Equip::SLOT_CARGO) continue;
		int stock = station->GetEquipmentStock(static_cast<Equip::Type>(i));
		if (!stock) continue;
		Gui::Label *l = new Gui::Label(EquipType::types[i].name);
		innerbox->Add(l,0,num*YSEP);
		innerbox->Add(new Gui::SolidButton(), 275, num*YSEP);
		innerbox->Add(new Gui::SolidButton(), 300, num*YSEP);
		char buf[128];
		snprintf(buf, sizeof(buf), "$%d", station->GetPrice(static_cast<Equip::Type>(i)));
		innerbox->Add(new Gui::Label(buf), 200, num*YSEP);
		snprintf(buf, sizeof(buf), "%dt", EquipType::types[i].mass);
		innerbox->Add(new Gui::Label(buf), 370, num*YSEP);
		num++;
	}
	innerbox->ShowAll();

	fbox->Add(new Gui::Label("Item"), 0, 0);
	fbox->Add(new Gui::Label("Price"), 200, 0);
	fbox->Add(new Gui::Label("Fit"), 275, 0);
	fbox->Add(new Gui::Label("Remove"), 300, 0);
	fbox->Add(new Gui::Label("Wt"), 370, 0);
	fbox->Add(portal, 0, YSEP);
	fbox->Add(scroll, 455, YSEP);
	portal->Add(innerbox);
	portal->ShowAll();
	fbox->ShowAll();
	ADD_VIDEO_WIDGET;

	Gui::Fixed::ShowAll();
}

////////////////////////////////////////////////////////////////////

class StationViewShipView: public StationSubView {
public:
	StationViewShipView(const ShipFlavour *f): StationSubView() {
		m_flavour = *f;
		m_sbreModel = sbreLookupModelByName(ShipType::types[f->type].sbreModelName);
		m_ondraw3dcon = Pi::spaceStationView->onDraw3D.connect(
				sigc::mem_fun(this, &StationViewShipView::Draw3D));
	}
	virtual ~StationViewShipView() {
		m_ondraw3dcon.disconnect();
	}
	virtual void ShowAll();
	void Draw3D();
private:
	ShipFlavour m_flavour;
	int m_sbreModel;
	sigc::connection m_ondraw3dcon;
};

void StationViewShipView::Draw3D()
{
	ObjParams params = {
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },

		{	// pColor[3]
		{ { 1.0f, 0.0f, 1.0f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 },
		{ { 0.8f, 0.6f, 0.5f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 },
		{ { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }, { 0, 0, 0 }, 0 } },

		// pText[3][256]	
		{ "IR-L33T", "ME TOO" },
	};

	m_flavour.ApplyTo(&params);

	static float rot1, rot2;
	rot1 += .5*Pi::GetFrameTime();
	rot2 += Pi::GetFrameTime();
	glClearColor(0,.2,.4,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	const float bx = 5;
	const float by = 40;
	Gui::Screen::EnterOrtho();
	glColor3f(0,0,0);
	glBegin(GL_QUADS); {
		glVertex2f(bx,by);
		glVertex2f(bx,by+400);
		glVertex2f(bx+400,by+400);
		glVertex2f(bx+400,by);
	} glEnd();
	Gui::Screen::LeaveOrtho();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-.5, .5, -.5, .5, 1.0f, 10000.0f);
	glDepthRange (0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	float lightCol[] = { 1,1,1 };
	float lightDir[] = { 1,1,0 };

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	sbreSetDirLight (lightCol, lightDir);
	glViewport(bx, Pi::GetScrHeight() - by - 400, 400, 400);
	
	matrix4x4d rot = matrix4x4d::RotateXMatrix(rot1);
	rot.RotateY(rot2);
	Matrix m;

	vector3d p(0, 0, -2 * sbreGetModelRadius(m_sbreModel));
	sbreSetDepthRange (Pi::GetScrWidth()*0.5f, 0.0f, 1.0f);
	sbreRenderModel(&p.x, &rot[0], m_sbreModel, &params);
	glPopAttrib();
}

void StationViewShipView::ShowAll()
{
	const ShipType &t = ShipType::types[m_flavour.type];
	DeleteAllChildren();

	SetTransparency(true);
	Add(new Gui::Label("Nothing to see yet"), 10, 10);
	Gui::Button *b = new Gui::SolidButton();
	b->onClick.connect(sigc::mem_fun(this, &StationViewShipView::GoBack));
	Add(b,680,470);
	Add(new Gui::Label("Go back"), 700, 470);

	const float YSEP = Gui::Screen::GetFontHeight() * 1.5f;
	float y = 40;
	Add(new Gui::Label("Ship type"), 450, y);
	Add(new Gui::Label(t.name), 600, y);
	y+=YSEP;
	Add(new Gui::Label("Price"), 450, y);
	Add(new Gui::Label(stringf(64, "$%d", m_flavour.price)), 600, y);
	y+=YSEP;
	Add(new Gui::Label("Registration id"), 450, y);
	Add(new Gui::Label(m_flavour.regid), 600, y);
	y+=YSEP;
	y+=YSEP;
	Add(new Gui::Label("Weight empty"), 450, y);
	Add(new Gui::Label(stringf(64, "%d t", t.hullMass)), 600, y);
	y+=YSEP;
	Add(new Gui::Label("Weight fully loaded"), 450, y);
	Add(new Gui::Label(stringf(64, "%d t", t.hullMass + t.capacity)), 600, y);
	y+=YSEP;
	Add(new Gui::Label("Capacity"), 450, y);
	Add(new Gui::Label(stringf(64, "%d t", t.capacity)), 600, y);
	y+=YSEP;
	y+=YSEP;
	// forward accel
	float accel = t.linThrust[ShipType::THRUSTER_REAR] / (-9.81*1000.0*(t.hullMass));
	Add(new Gui::Label("Forward accel (empty)"), 450, y);
	Add(new Gui::Label(stringf(64, "%.1f G", accel)), 600, y);
	y+=YSEP;
	accel = t.linThrust[ShipType::THRUSTER_REAR] / (-9.81*1000.0*(t.hullMass + t.capacity));
	Add(new Gui::Label("Forward accel (laden)"), 450, y);
	Add(new Gui::Label(stringf(64, "%.1f G", accel)), 600, y);
	y+=YSEP;
	// rev accel
	accel = t.linThrust[ShipType::THRUSTER_FRONT] / (9.81*1000.0*(t.hullMass));
	Add(new Gui::Label("Reverse accel (empty)"), 450, y);
	Add(new Gui::Label(stringf(64, "%.1f G", accel)), 600, y);
	y+=YSEP;
	accel = t.linThrust[ShipType::THRUSTER_FRONT] / (9.81*1000.0*(t.hullMass + t.capacity));
	Add(new Gui::Label("Reverse accel (laden)"), 450, y);
	Add(new Gui::Label(stringf(64, "%.1f G", accel)), 600, y);
	y+=YSEP;



	//ADD_VIDEO_WIDGET;

	Gui::Fixed::ShowAll();
}

////////////////////////////////////////////////////////////////////

class StationBuyShipsView: public StationSubView {
public:
	StationBuyShipsView();
	virtual ~StationBuyShipsView() {
		m_onShipsForSaleChangedConnection.disconnect();
	}
	virtual void ShowAll();
private:
	void ViewShip(int idx) {
		HideChildren();
		SetTransparency(true);
		SpaceStation *station = Pi::player->GetDockedWith();
		StationSubView *v = new StationViewShipView(&station->GetShipsOnSale()[idx]);
		Add(v, 0, 0);
		v->ShowAll();
		printf("view ship %d\n", idx);
	}
	sigc::connection m_onShipsForSaleChangedConnection;
};

StationBuyShipsView::StationBuyShipsView(): StationSubView()
{
	SpaceStation *station = Pi::player->GetDockedWith();
	m_onShipsForSaleChangedConnection = station->onShipsForSaleChanged.connect(
			sigc::mem_fun(this, &StationBuyShipsView::ShowAll));
	SetTransparency(false);
}

void StationBuyShipsView::ShowAll()
{
	DeleteAllChildren();

	SpaceStation *station = Pi::player->GetDockedWith();
	assert(station);
	SetTransparency(false);
	{
		Add(new Gui::Label("Nothing to see yet"), 10, 10);
	}
	Gui::Button *b = new Gui::SolidButton();
	b->onClick.connect(sigc::mem_fun(this, &StationBuyShipsView::GoBack));
	Add(b,680,470);
	Add(new Gui::Label("Go back"), 700, 470);

	Gui::Fixed *fbox = new Gui::Fixed(470, 400);
	Add(fbox, 320, 40);

	Gui::VScrollBar *scroll = new Gui::VScrollBar();
	Gui::VScrollPortal *portal = new Gui::VScrollPortal(450,400);
	scroll->SetAdjustment(&portal->vscrollAdjust);

	std::vector<ShipFlavour> &ships = station->GetShipsOnSale();
	int NUM_ITEMS = ships.size();
	const float YSEP = Gui::Screen::GetFontHeight() * 1.5f;

	int num = 0;
	Gui::Fixed *innerbox = new Gui::Fixed(450, NUM_ITEMS*YSEP);
	for (std::vector<ShipFlavour>::iterator i = ships.begin(); i!=ships.end(); ++i) {
		Gui::Label *l = new Gui::Label(ShipType::types[(*i).type].name);
		innerbox->Add(l,0,num*YSEP);
		char buf[128];
		snprintf(buf, sizeof(buf), "$%d", (*i).price);
		innerbox->Add(new Gui::Label(buf), 200, num*YSEP);
		snprintf(buf, sizeof(buf), "%dt", -1);
		innerbox->Add(new Gui::Label(buf), 275, num*YSEP);
		Gui::SolidButton *b = new Gui::SolidButton();
		b->onClick.connect(sigc::bind(sigc::mem_fun(this, &StationBuyShipsView::ViewShip), num));
		innerbox->Add(b, 370, num*YSEP);
		innerbox->Add(new Gui::SolidButton(), 410, num*YSEP);
		num++;
	}
	innerbox->ShowAll();

	fbox->Add(new Gui::Label("Ship"), 0, 0);
	fbox->Add(new Gui::Label("Price"), 200, 0);
	fbox->Add(new Gui::Label("Part exchange"), 275, 0);
	fbox->Add(new Gui::Label("View"), 370, 0);
	fbox->Add(new Gui::Label("Buy"), 410, 0);
	fbox->Add(portal, 0, YSEP);
	fbox->Add(scroll, 455, YSEP);
	portal->Add(innerbox);
	portal->ShowAll();
	fbox->ShowAll();
	ADD_VIDEO_WIDGET;

	Gui::Fixed::ShowAll();
}


////////////////////////////////////////////////////////////////////

class StationShipyardView: public StationSubView {
public:
	StationShipyardView();
	virtual void ShowAll();
private:
	void GotoUpgradesView() {
		HideChildren();
		SetTransparency(true);
		StationSubView *v = new StationShipUpgradesView();
		Add(v, 0, 0);
		v->ShowAll();
	}
	void GotoBuyShipsView() {
		HideChildren();
		SetTransparency(true);
		StationSubView *v = new StationBuyShipsView();
		Add(v, 0, 0);
		v->ShowAll();
	}
};

StationShipyardView::StationShipyardView(): StationSubView()
{
	SetTransparency(false);
}

void StationShipyardView::ShowAll()
{
	DeleteAllChildren();

	SpaceStation *station = Pi::player->GetDockedWith();
	assert(station);
	SetTransparency(false);
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "Welcome to %s shipyard", station->GetLabel().c_str());
		Add(new Gui::Label(buf), 10, 10);
	}
	
	Gui::Button *backButton = new Gui::SolidButton();
	backButton->onClick.connect(sigc::mem_fun(this, &StationShipyardView::GoBack));
	Add(backButton,680,470);
	Add(new Gui::Label("Go back"), 700, 470);

	Gui::SolidButton *b = new Gui::SolidButton();
	b->SetShortcut(SDLK_1, KMOD_NONE);
	b->onClick.connect(sigc::mem_fun(this, &StationShipyardView::GotoUpgradesView));
	Add(b, 340, 240);
	Gui::Label *l = new Gui::Label("Ship equipment");
	Add(l, 365, 240);
	
	b = new Gui::SolidButton();
	b->SetShortcut(SDLK_1, KMOD_NONE);
	b->onClick.connect(sigc::mem_fun(this, &StationShipyardView::GotoBuyShipsView));
	Add(b, 340, 300);
	l = new Gui::Label("New and reconditioned ships");
	Add(l, 365, 300);
	
	ADD_VIDEO_WIDGET;

	Gui::Fixed::ShowAll();
}

/////////////////////////////////////////////////////////////////////

SpaceStationView::SpaceStationView(): View()
{
	Gui::Label *l = new Gui::Label("Comms Link");
	l->SetColor(1,.7,0);
	m_rightRegion2->Add(l, 10, 0);
}

void SpaceStationView::OnClickRequestLaunch()
{
	Pi::player->SetDockedWith(0,0);
	Pi::SetView(Pi::worldView);
}

void SpaceStationView::GotoCommodities()
{
	HideChildren();
	SetTransparency(true);
	StationSubView *v = new StationCommoditiesView();
	Add(v, 0, 0);
	v->ShowAll();
}

void SpaceStationView::GotoShipyard()
{
	HideChildren();
	SetTransparency(true);
	StationSubView *v = new StationShipyardView();
	Add(v, 0, 0);
	v->ShowAll();
}

void SpaceStationView::OnSwitchTo()
{
	SetTransparency(false);
	DeleteAllChildren();
	SpaceStation *station = Pi::player->GetDockedWith();
	assert(station);
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "Welcome to %s", station->GetLabel().c_str());
		Add(new Gui::Label(buf), 10, 10);
	}
	Gui::Label *l = new Gui::Label("Hello friend! Thankyou for docking with this space station!\n"
	"We regret to inform you that due to a spacetime fissure you have "
	"ended up in the terrible mirror universe of boringness, and that there "
	"is nothing to do. We hope to have things back to normal within a "
	"few weeks, and in the mean time would like to offer our apologies for "
	"any loss of earnings, immersion or lunch.  "
	"Currently the usual space station services are not available, but we "
	"can offer you this promotional message from one of the station's sponsors:\n"
	"                       DIET STEAKETTE: IT'S BAD");

	Gui::Fixed *fbox = new Gui::Fixed(450, 400);
	fbox->Add(l, 0, 0);
	Add(fbox, 320, 40);
	fbox->ShowAll();

	Gui::SolidButton *b = new Gui::SolidButton();
	b->SetShortcut(SDLK_1, KMOD_NONE);
	b->onClick.connect(sigc::mem_fun(this, &SpaceStationView::OnClickRequestLaunch));
	Add(b, 340, 240);
	l = new Gui::Label("Request Launch");
	Add(l, 365, 240);

	b = new Gui::SolidButton();
	b->SetShortcut(SDLK_2, KMOD_NONE);
	b->onClick.connect(sigc::mem_fun(this, &SpaceStationView::GotoShipyard));
	Add(b, 340, 300);
	l = new Gui::Label("Shipyard");
	Add(l, 365, 300);

	b = new Gui::SolidButton();
	b->SetShortcut(SDLK_3, KMOD_NONE);
	b->onClick.connect(sigc::mem_fun(this, &SpaceStationView::GotoCommodities));
	Add(b, 340, 360);
	l = new Gui::Label("Commodity market");
	Add(l, 365, 360);

	ADD_VIDEO_WIDGET;

	Gui::Fixed::ShowAll();
}

void SpaceStationView::Draw3D()
{
	onDraw3D.emit();
}

void SpaceStationView::Update()
{
}
