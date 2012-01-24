#ifndef _VIEW_H
#define _VIEW_H

#include "libs.h"
#include "Serializer.h"
#include "gui/Gui.h"

/*
 * For whatever draws crap into the main area of the screen.
 * Eg:
 *  game 3d view
 *  system map
 *  sector map
 */
class View: public Gui::Fixed {
public:
	View(): Gui::Fixed(float(Gui::screen->GetWidth()), float(Gui::screen->GetHeight()-64)) {
		Gui::screen->AddBaseWidget(this, 0, 0);
		
		m_rightButtonBar = new Gui::Fixed(128, 26);
		m_rightButtonBar->SetBgColor(.65f, .65f, .65f, 1.0f);
		Gui::screen->AddBaseWidget(m_rightButtonBar, Gui::screen->GetWidth()-128, Gui::screen->GetHeight()-26);

		m_rightRegion2 = new Gui::Fixed(126, 17);
		m_rightRegion2->SetTransparency(true);
		Gui::screen->AddBaseWidget(m_rightRegion2, Gui::screen->GetWidth()-127, Gui::screen->GetHeight()-45);
		
		m_rightRegion1 = new Gui::Fixed(122, 17);
		m_rightRegion1->SetTransparency(true);
		Gui::screen->AddBaseWidget(m_rightRegion1, Gui::screen->GetWidth()-123, Gui::screen->GetHeight()-62);
	}
	virtual ~View() {
		Gui::screen->RemoveBaseWidget(m_rightButtonBar);
		Gui::screen->RemoveBaseWidget(m_rightRegion2);
		Gui::screen->RemoveBaseWidget(m_rightRegion1);
		delete m_rightButtonBar;
		delete m_rightRegion2;
		delete m_rightRegion1;
	}
	virtual void ShowAll() {
		m_rightButtonBar->ShowAll();
		m_rightRegion2->ShowAll();
		m_rightRegion1->ShowAll();
		Gui::Fixed::ShowAll();
	}
	virtual void HideAll() {
		m_rightButtonBar->HideAll();
		m_rightRegion2->HideAll();
		m_rightRegion1->HideAll();
		Gui::Fixed::HideAll();
	}
	// called before Gui::Draw will call widget ::Draw methods.
	virtual void Draw3D() = 0;
	// for checking key states, mouse crud
	virtual void Update() = 0;
	virtual void Save(Serializer::Writer &wr) {}
	virtual void Load(Serializer::Reader &rd) {}
	virtual void OnSwitchTo() = 0;
protected:
	// each view can put some buttons in the bottom right of the cpanel
	Gui::Fixed *m_rightButtonBar;
	Gui::Fixed *m_rightRegion1;
	Gui::Fixed *m_rightRegion2;
};

#endif /* _VIEW_H */
