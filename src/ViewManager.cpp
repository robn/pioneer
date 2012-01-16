#include "ViewManager.h"

#include "ShipCpanel.h"
#include "SectorView.h"
#include "WorldView.h"
#include "GalacticView.h"
#include "SystemView.h"
#include "SystemInfoView.h"
#include "SpaceStationView.h"
#include "InfoView.h"
#if OBJECTVIEWER
#include "ObjectViewerView.h"
#endif

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

// XXX should not call HideAll()/ShowAll(), but that requires all the base
// container stuff to be fixed first
void ViewManager::SwitchTo(View *v)
{
	if (m_currentView) m_currentView->HideAll();
	m_currentView = v;
	if (m_currentView) {
		m_currentView->OnSwitchTo();
		m_currentView->ShowAll();
	}
}
