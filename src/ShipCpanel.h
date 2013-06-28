// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _SHIPCPANEL_H
#define _SHIPCPANEL_H

#include "libs.h"
#include "gui/Gui.h"
#include "ShipCpanelMultiFuncDisplays.h"
#include "Ship.h"
#include "Serializer.h"
#include "Game.h"
#include "WorldView.h"
#include "Sound.h"

class Body;
class SpaceStation;
namespace Graphics { class Renderer; }

class ShipCpanel: public Gui::Fixed {
public:
	ShipCpanel(Game *game, Graphics::Renderer *r);
    ShipCpanel(Serializer::Reader &rd, Game *game, Graphics::Renderer *r);
	virtual ~ShipCpanel();
	virtual void Draw();
	void Update();
	MsgLogWidget *MsgLog() { return m_msglog; }

	void TimeStepUpdate(float step);

	void Save(Serializer::Writer &wr);

	enum OverlayTextPos {
		OVERLAY_TOP_LEFT,
		OVERLAY_TOP_RIGHT,
		OVERLAY_BOTTOM_LEFT,
		OVERLAY_BOTTOM_RIGHT
	};
	void SetOverlayText(OverlayTextPos pos, const std::string &text);
	void SetOverlayToolTip(OverlayTextPos pos, const std::string &text);
	void ClearOverlay();

private:
	void InitObject();
	void OnRotationDampingChanged();

	enum MapView { MAP_SECTOR, MAP_SYSTEM, MAP_INFO, MAP_GALACTIC };

	void OnChangeCamView(Gui::MultiStateImageButton *b);
	void OnChangeToMapView(Gui::MultiStateImageButton *b);
	void OnChangeMapView(enum MapView);
	void OnChangeInfoView(Gui::MultiStateImageButton *b);
	void OnClickTimeaccel(Game::TimeAccel val);
	void OnClickComms(Gui::MultiStateImageButton *b);
	void OnClickRotationDamping(Gui::MultiStateImageButton *b);

	void OnUserChangeMultiFunctionDisplay(multifuncfunc_t f);
	void ChangeMultiFunctionDisplay(multifuncfunc_t selected);
	void OnMultiFuncGrabFocus(multifuncfunc_t);
	void OnMultiFuncUngrabFocus(multifuncfunc_t);
	void HideMapviewButtons();

	Game *m_game;

	enum MapView m_currentMapView;
	multifuncfunc_t m_userSelectedMfuncWidget;
	Gui::Label *m_clock;

	sigc::connection m_connOnRotationDampingChanged;

	MultiFuncSelectorWidget *m_mfsel;
	ScannerWidget *m_scanner;
	MsgLogWidget *m_msglog;
	UseEquipWidget *m_useEquipWidget;
	Gui::MultiStateImageButton *m_camButton;
	Gui::RadioGroup *m_leftButtonGroup, *m_rightButtonGroup;
	Gui::ImageRadioButton *m_timeAccelButtons[6];
	Gui::Widget *m_mapViewButtons[4];
	Gui::MultiStateImageButton *m_rotationDampingButton;
	Gui::Image *m_alertLights[3];

	Gui::Label *m_overlay[4];

	sigc::connection m_connOnShipAlertStateChanged;
	void ShipAlertStateChanged(Ship *ship);
	Ship::AlertState m_prevAlertState;

	sigc::connection m_connOnShipHyperspaceCountdownStarted;
	sigc::connection m_connOnShipHyperspaceCountdownAborted;
	void OnShipHyperspaceCountdownStarted(Ship *ship);
	void OnShipHyperspaceCountdownAborted(Ship *ship);
	Sound::Event m_soundHyperdrive;

	sigc::connection m_connOnStationDockingClearanceExpired;
	void OnStationDockingClearanceExpired(const SpaceStation *station, const Ship *ship);

};

#endif /* _SHIP_CPANEL_H */
