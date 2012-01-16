#ifndef _VIEWMANAGER_H
#define _VIEWMANAGER_H

#include "buildopts.h"
#include "SmartPtr.h"

class ShipCpanel;
class View;
class SectorView;
class WorldView;
class GalacticView;
class SystemView;
class SystemInfoView;
class SpaceStationView;
class InfoView;
#if OBJECTVIEWER
class ObjectViewerView;
#endif

class ViewManager {
public:
	ViewManager();
	~ViewManager();

	void Update();
	void Draw();
	void Draw3D();

	// XXX should be restricted to our own views
	void SwitchTo(View *v);

	View *GetCurrentView() const { return m_currentView; }

	ShipCpanel *GetShipCpanel() const { return m_cpanel.Get(); }

	SectorView       *GetSectorView()       const { return m_sectorView.Get(); }
	WorldView        *GetWorldView()        const { return m_worldView.Get(); }
	GalacticView     *GetGalacticView()     const { return m_galacticView.Get(); }
	SystemView       *GetSystemView()       const { return m_systemView.Get(); }
	SystemInfoView   *GetSystemInfoView()   const { return m_systemInfoView.Get(); }
	SpaceStationView *GetSpaceStationView() const { return m_spaceStationView.Get(); }
	InfoView         *GetInfoView()         const { return m_infoView.Get(); }
#if OBJECTVIEWER
	ObjectViewerView *GetObjectViewerView() const { return m_objectViewerView.Get(); }
#endif

private:
	View *m_currentView;

	ScopedPtr<ShipCpanel>       m_cpanel;

	ScopedPtr<SectorView>       m_sectorView;
	ScopedPtr<WorldView>        m_worldView;
	ScopedPtr<GalacticView>     m_galacticView;
	ScopedPtr<SystemView>       m_systemView;
	ScopedPtr<SystemInfoView>   m_systemInfoView;
	ScopedPtr<SpaceStationView> m_spaceStationView;
	ScopedPtr<InfoView>         m_infoView;
#if OBJECTVIEWER
	ScopedPtr<ObjectViewerView> m_objectViewerView;
#endif
};

#endif
