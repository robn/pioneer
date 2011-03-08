#ifndef _SYSTEMINFOVIEW_H
#define _SYSTEMINFOVIEW_H

#include "libs.h"
#include "Gui.h"
#include "View.h"
#include "GeoSphere.h"

class StarSystem;
class SBody;

class SystemInfoView: public View {
public:
	SystemInfoView();
	virtual ~SystemInfoView();
	virtual void Update();
	virtual void Draw3D();
	virtual void OnSwitchTo() {}
private:
	StarSystem *m_system;
	SBody *m_selectedObject;
    GeoSphere *m_geosphere;
};

#endif /* _SYSTEMINFOVIEW_H */
