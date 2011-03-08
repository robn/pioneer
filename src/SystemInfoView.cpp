#include "SystemInfoView.h"
#include "Pi.h"
#include "SectorView.h"
#include "StarSystem.h"

SystemInfoView::SystemInfoView()
{
	m_system = 0;
	SetTransparency(true);

	ResetViewpoint();
}

SystemInfoView::~SystemInfoView()
{
}

void SystemInfoView::ResetViewpoint()
{
	m_selectedObject = 0;
}

static const GLfloat fogDensity = 0.1;
static const GLfloat fogColor[4] = { 0,0,0,1.0 };

void SystemInfoView::Draw3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, Pi::GetScrAspect(), 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int sector_x, sector_y, system_idx;
	Pi::sectorView->GetSelectedSystem(&sector_x, &sector_y, &system_idx);
	if (m_system) {
		if (!m_system->IsSystem(sector_x, sector_y, system_idx)) {
			delete m_system;
			m_system = 0;
			ResetViewpoint();
		}
	}

	if (!m_system) m_system = new StarSystem(sector_x, sector_y, system_idx);

	glTranslatef(0,0,-10.0);

	float radius = (float)m_system->rootBody->GetRadius()*200.0/AU;

	glColor3f(1,1,1);
	glBegin(GL_TRIANGLE_FAN);
	for (float ang=0; ang<2.0f*M_PI; ang+=M_PI*0.05f) {
		vector3f p = vector3f(radius*sin(ang), -radius*cos(ang), 0);
		glVertex3fv(&p.x);
	}
	glEnd();
}

void SystemInfoView::Update()
{
}
