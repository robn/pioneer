// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "GameSound.h"
#include "Game.h"
#include "GameEvents.h"
#include "Sound.h"
#include "Ship.h"

GameSound::GameSound(Game *game)
{
	GameEvents &s(game->Events());

	m_connections.push_back(s.onShipGearDown.connect(sigc::ptr_fun(&GameSound::ShipGearDown)));
	m_connections.push_back(s.onShipGearUp.connect(sigc::ptr_fun(&GameSound::ShipGearUp)));
}

GameSound::~GameSound()
{
	for (std::vector<sigc::connection>::iterator i = m_connections.begin(); i != m_connections.end(); ++i)
		(*i).disconnect();
}

void GameSound::ShipGearDown(Ship *ship)
{
	Sound::BodyMakeNoise(ship, "UC_out", 1.0f);
}

void GameSound::ShipGearUp(Ship *ship)
{
	Sound::BodyMakeNoise(ship, "UC_in", 1.0f);
}
