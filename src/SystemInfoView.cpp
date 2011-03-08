#include "SystemInfoView.h"
#include "Pi.h"
#include "SectorView.h"
#include "StarSystem.h"

SystemInfoView::SystemInfoView()
{
	m_system = 0;
    m_selectedObject = 0;
    m_geosphere = 0;

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
	gluPerspective(50, Pi::GetScrAspect(), 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float lightPos[4] = { 0,0,0,0 };
	float lightCol[4] = { 1.0, 1.0, 1.0, 0 };
	float ambCol[4] = { 1.0,1.0,1.0,0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambCol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightCol);
	glEnable(GL_LIGHT0);

	glTranslatef(0,0, -(m_selectedObject->GetRadius()/2000000));
	vector3d campos(0,0,0);

    m_geosphere->Render(campos, m_selectedObject->GetRadius(), 200.0);
}

void SystemInfoView::Update()
{
}
