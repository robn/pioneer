// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

#include <sigc++/sigc++.h>

class Ship;
class Missile;

class GameEvents {
public:

	sigc::signal<void,Ship*>          onShipGearDown;
	sigc::signal<void,Ship*>          onShipGearUp;
	sigc::signal<void,Ship*,Missile*> onShipMissileLaunched;
	sigc::signal<void,Ship*>          onShipAlertStateChanged;
	sigc::signal<void,Ship*>          onShipHyperspaceCountdownStarted;
	sigc::signal<void,Ship*>          onShipHyperspaceCountdownAborted;
};

#endif
