#include "libs.h"
#include "gui/Gui.h"
#include "Pi.h"
#include "GalacticView.h"
#include "SystemInfoView.h"
#include "Player.h"
#include "Serializer.h"
#include "SectorView.h"
#include "galaxy/Sector.h"
#include "galaxy/Galaxy.h"
#include "Lang.h"
#include "StringF.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"

using namespace Graphics;

GalacticView::GalacticView()
{
	m_texture.Reset(new UITexture(Pi::galaxy->GetGalaxyBitmap()));

	SetTransparency(true);
	m_zoom = 1.0f;

	m_zoomInButton = new Gui::ImageButton("icons/zoom_in.png");
	m_zoomInButton->SetToolTip(Lang::ZOOM_IN);
	Add(m_zoomInButton, 700, 5);
	
	m_zoomOutButton = new Gui::ImageButton("icons/zoom_out.png");
	m_zoomOutButton->SetToolTip(Lang::ZOOM_OUT);
	Add(m_zoomOutButton, 732, 5);
	
	m_scaleReadout = new Gui::Label("");
	Add(m_scaleReadout, 500.0f, 10.0f);

	Gui::Screen::PushFont("OverlayFont");
	m_labels = new Gui::LabelSet();
	Add(m_labels, 0, 0);
	Gui::Screen::PopFont();

	m_onMouseButtonDown = 
		Pi::onMouseButtonDown.connect(sigc::mem_fun(this, &GalacticView::MouseButtonDown));
}

GalacticView::~GalacticView()
{
	m_onMouseButtonDown.disconnect();
}

void GalacticView::Save(Serializer::Writer &wr)
{
}

void GalacticView::Load(Serializer::Reader &rd)
{
}

static void dummy() {}

void GalacticView::PutLabels(vector3d offset)
{
	Gui::Screen::EnterOrtho();
	glColor3f(1,1,1);

	for (const Galaxy::FeatureLabel *feature = Galaxy::GALACTIC_FEATURES; feature->text; ++feature) {
		const vector3d p = m_zoom * (feature->position / Pi::galaxy->GetRadius() + offset);
		vector3d pos;
		if (Gui::Screen::Project(p, pos)) {
			m_labels->Add(feature->text, sigc::ptr_fun(&dummy), float(pos.x), float(pos.y));
		}
	}

	Gui::Screen::LeaveOrtho();
}


void GalacticView::Draw3D()
{
	const vector3f solpos = Pi::galaxy->GetSolPosition();
	const vector3f sectorpos = Pi::sectorView->GetPosition();
	const vector3f pos(
			(sectorpos.x*Sector::SIZE + solpos.x) / Pi::galaxy->GetRadius(),
			(-sectorpos.y*Sector::SIZE + solpos.y) / Pi::galaxy->GetRadius(),
			0.0f
		);

	m_renderer->SetOrthographicProjection(-Pi::GetScrAspect(), Pi::GetScrAspect(), 1.f, -1.f, -1.f, 1.f);
	m_renderer->ClearScreen();
	m_renderer->SetDepthTest(false);
	m_renderer->SetBlendMode(BLEND_SOLID);

	// XXX fixed function combiner
	glEnable(GL_TEXTURE_2D);
	m_texture->Bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	m_texture->Unbind();
	glDisable(GL_TEXTURE_2D);
	
	//apply zoom
	m_renderer->SetTransform(
		matrix4x4f::Identity() *
		matrix4x4f::ScaleMatrix(m_zoom, m_zoom, 0.f) *
		matrix4x4f::Translation(-pos.x, -pos.y, 0.f));

	// galaxy image
	VertexArray va(ATTRIB_POSITION | ATTRIB_UV0);
	const float w = 1.0;
	const float h = 1.0;
	// XXX 2d verts
	va.Add(vector3f(-1.0,-1.0, 0.f), vector2f(0.f,0.f));
	va.Add(vector3f(-1.f, 1.f, 0.f), vector2f(0.f,h));
	va.Add(vector3f( 1.0,-1.0, 0.f), vector2f(w,0.f));
	va.Add(vector3f( 1.f, 1.f, 0.f), vector2f(w,h));

	Material m;
	m.unlit = true;
	m.texture0 = m_texture.Get();
	m_renderer->DrawTriangles(&va, &m, TRIANGLE_STRIP);

	// "you are here" dot
	Color green(0.f, 1.f, 0.f, 1.f);
	vector2f offs(pos.x, pos.y);
	m_renderer->DrawPoints2D(1, &offs, &green, 3.f);

	// scale at the top
	m_renderer->SetTransform(matrix4x4f::Identity());
	Color white(1.f);
	const vector2f vts[] = {
		vector2f(-0.25,-0.93),
		vector2f(-0.25,-0.94),
		vector2f(0.25,-0.94),
		vector2f(0.25,-0.93)
	};
	m_renderer->DrawLines2D(4, vts, white, LINE_STRIP);

	m_labels->Clear();
	PutLabels(-vector3d(pos));

	m_renderer->SetDepthTest(true);
}
	
void GalacticView::Update()
{
	const float frameTime = Pi::GetFrameTime();
	
	if (m_zoomInButton->IsPressed()) m_zoom *= pow(4.0f, frameTime);
	if (m_zoomOutButton->IsPressed()) m_zoom *= pow(0.25f, frameTime);
	// XXX ugly hack checking for console here
	if (!Pi::IsConsoleActive()) {
		if (Pi::KeyState(SDLK_EQUALS)) m_zoom *= pow(4.0f, frameTime);
		if (Pi::KeyState(SDLK_MINUS)) m_zoom *= pow(0.25f, frameTime);
	}
	m_zoom = Clamp(m_zoom, 0.5f, 100.0f);

	m_scaleReadout->SetText(stringf(Lang::INT_LY, formatarg("scale", int(0.5*Pi::galaxy->GetRadius()/m_zoom))));
}

void GalacticView::MouseButtonDown(int button, int x, int y)
{
	if (this == Pi::GetView()) {
		const float ft = Pi::GetFrameTime();
		if (Pi::MouseButtonState(SDL_BUTTON_WHEELDOWN)) 
				m_zoom *= pow(0.25f, ft);
		if (Pi::MouseButtonState(SDL_BUTTON_WHEELUP)) 
				m_zoom *= pow(4.0f, ft);
	}
}

