#include "ViewManager.h"

#include "ShipCpanel.h"
#include "SectorView.h"
#include "WorldView.h"
#include "GalacticView.h"
#include "SystemView.h"
#include "SystemInfoView.h"
#include "SpaceStationView.h"
#include "InfoView.h"
#if WITH_OBJECTVIEWER
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
#if WITH_OBJECTVIEWER
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
#if WITH_OBJECTVIEWER
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

void ViewManager::SwitchTo(View::Type type)
{
	// XXX icky but really easy :)
	View *v = 0;
	switch (type) {
		case View::SECTOR:       v = m_sectorView.Get();       break;
		case View::WORLD:        v = m_worldView.Get();        break;
		case View::GALACTIC:     v = m_galacticView.Get();     break;
		case View::SYSTEM:       v = m_systemView.Get();       break;
		case View::SYSTEMINFO:   v = m_systemInfoView.Get();   break;
		case View::SPACESTATION: v = m_spaceStationView.Get(); break;
		case View::INFO:         v = m_infoView.Get();         break;
#if WITH_OBJECTVIEWER
		case View::OBJECTVIEWER: v = m_objectViewerView.Get(); break;
#endif
		default:
			assert(0);
	}
	assert(v);

	// XXX should not call HideAll()/ShowAll(), but that requires all the base
	// container stuff to be fixed first
	if (m_currentView) m_currentView->HideAll();
	m_currentView = v;
	if (m_currentView) {
		m_currentView->OnSwitchTo();
		m_currentView->ShowAll();
	}
}
