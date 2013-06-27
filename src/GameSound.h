// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef GAMESOUND_H
#define GAMESOUND_H

#include "GameEvents.h"
#include <vector>

class Game;

class GameSound {
public:
	GameSound(Game *game);
	~GameSound();

private:
	static void ShipGearDown(Ship *ship);
	static void ShipGearUp(Ship *ship);
	static void ShipMissileLaunched(Ship *ship, Missile *missile);

	std::vector<sigc::connection> m_connections;
};

#endif
