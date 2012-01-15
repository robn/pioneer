#include "ViewManager.h"

ViewManager::ViewManager() :
	m_currentView(0)
{
	m_cpanel.Reset(new ShipCpanel());

	m_sectorView.Reset(new SectorView());
	m_worldView.Reset(new WorldView());
	m_galacticView.Reset(new GalacticView());
	m_systemView.Reset(new SystemView());
	m_systemInfoView.Reset(new SystemInfoView());
	m_spaceStationView.Reset(new SpaceStationView());
	m_infoView.Reset(new InfoView());
#if OBJECTVIEWER
	m_objectViewerView.Reset(new ObjectViewerView());
#endif
}

ViewManager::~ViewManager()
{
}

void ViewManager::Update()
{
	m_cpanel->Update();

	m_sectorView->Update();
	m_worldView->Update();
	m_galacticView->Update();
	m_systemView->Update();
	m_systemInfoView->Update();
	m_spaceStationView->Update();
	m_infoView->Update();
#if OBJECTVIEWER
	m_objectViewerView->Update();
#endif
}

void ViewManager::Draw()
{
	if (m_currentView)
		m_currentView->Draw();
}

void ViewManager::Draw3D()
{
	if (m_currentView)
		m_currentView->Draw3D();
}
