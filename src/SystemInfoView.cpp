#include "SystemInfoView.h"
#include "Pi.h"
#include "SectorView.h"
#include "StarSystem.h"

SystemInfoView::SystemInfoView()
{
	m_system = 0;
    m_selectedObject = 0;
    m_geosphere = 0;
	m_rotation = 0;

	SetTransparency(true);
}

SystemInfoView::~SystemInfoView()
{
}

static const GLfloat fogDensity = 0.1;
static const GLfloat fogColor[4] = { 0,0,0,1.0 };

void SystemInfoView::Draw3D()
{
	int sector_x, sector_y, system_idx;
	Pi::sectorView->GetSelectedSystem(&sector_x, &sector_y, &system_idx);
	if (m_system) {
		if (!m_system->IsSystem(sector_x, sector_y, system_idx)) {
			delete m_system;
			m_system = 0;
			m_selectedObject = 0;
			if (m_geosphere) delete m_geosphere;
		}
	}

	if (!m_system) m_system = new StarSystem(sector_x, sector_y, system_idx);

	if (!m_geosphere) {
		m_selectedObject = m_system->m_bodies[3];
		m_geosphere = new GeoSphere(m_selectedObject);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-Pi::GetScrAspect(), Pi::GetScrAspect(), 1.0, -1.0, 0.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);

	glTranslatef(0, 0, -1);
	glRotatef(180, 0, 0, 1);

	glRotatef(-m_rotation, 0, 1, 0);
	vector3d campos(0,0,0);
	m_geosphere->Render(campos, m_selectedObject->GetRadius(), 4.0);
}

void SystemInfoView::Update()
{
	if ((m_rotation += 0.1f) == 360) m_rotation = 0;
}
