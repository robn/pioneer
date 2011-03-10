#include "SystemInfoView.h"
#include "Pi.h"
#include "SectorView.h"
#include "StarSystem.h"
#include "WorldView.h"

SystemInfoView::SystemInfoView()
{
	m_system = 0;
    m_selectedObject = 0;
    m_planet = 0;
	m_rotation = 0;

	SetTransparency(true);
}

SystemInfoView::~SystemInfoView()
{
}

static const GLfloat fogDensity = 0.1;
static const GLfloat fogColor[4] = { 0,0,0,1.0 };

extern vector3d last_viewCoords;
extern matrix4x4d last_viewTransform;

void SystemInfoView::Draw3D()
{
	int sector_x, sector_y, system_idx;
	Pi::sectorView->GetSelectedSystem(&sector_x, &sector_y, &system_idx);
	if (m_system) {
		if (!m_system->IsSystem(sector_x, sector_y, system_idx)) {
			delete m_system;
			m_system = 0;
			m_selectedObject = 0;
			if (m_planet) delete m_planet;
		}
	}

	if (!m_system) m_system = new StarSystem(sector_x, sector_y, system_idx);

	if (!m_planet) {
		m_selectedObject = m_system->m_bodies[3];
		m_planet = new Planet(m_selectedObject);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float znear, zfar;
	Pi::worldView->GetNearFarClipPlane(&znear, &zfar);
	// why the hell do i give these functions such big names..
	const float zoom = 0.9f; // angle of viewing = 2.0*atan(zoom);
	const float left = zoom * znear;
	const float fracH = left / Pi::GetScrAspect();
	//glOrtho(-left, left, -fracH, fracH, znear, zfar);
	//glFrustum(-left, left, -fracH, fracH, znear, zfar);
	glOrtho(-Pi::GetScrAspect()*100000.0, Pi::GetScrAspect()*100000.0, 100000.0, -100000.0, znear, zfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);

	glTranslatef(0,0,-100);

	/*
	glTranslatef(0, 0, -1);
	glRotatef(180, 0, 0, 1);

	glRotatef(-m_rotation, 0, 1, 0);
	vector3d campos(0,0,0);
	*/

	m_planet->Render(last_viewCoords, last_viewTransform);
}

void SystemInfoView::Update()
{
	if ((m_rotation += 0.1f) == 360) m_rotation = 0;
}
