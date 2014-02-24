// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

// helpers to send comms events through to Lua
// XXX remove once all game UI is moved out of core

#include "libs.h"

class Body;

namespace Comms {

void Message(const std::string &message, const std::string &from = std::string());
void Message(const std::string &message, Body *body);
void ImportantMessage(const std::string &message, const std::string &from = std::string());
void ImportantMessage(const std::string &message, Body *body);

}
