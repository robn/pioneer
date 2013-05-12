// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "MarketAgent.h"
#include "SaveLoad.h"
#include "Player.h"
#include "Pi.h"
#include "Lang.h"

SaveLoad::Object MarketAgent::Save() const {
	SaveLoad::Object so;
	so.Set("money", m_money);
	return so;
}

/* XXX DESERIALIZER
void MarketAgent::Load(SaveLoad::Reader &rd)
{
	m_money = rd.Int64();
}
*/

bool MarketAgent::SellTo(MarketAgent *other, Equip::Type t, bool verbose)
{
	if (other->CanBuy(t, verbose) && CanSell(t, verbose) && other->Pay(this, GetPrice(t), verbose)) {
		Sold(t);
		other->Bought(t);
		return true;
	} else return false;
}

bool MarketAgent::BuyFrom(MarketAgent *other, Equip::Type t, bool verbose)
{
	if (other->CanSell(t, verbose) && CanBuy(t, verbose) && Pay(other, GetPrice(t), verbose)) {
		other->Sold(t);
		Bought(t);
		return true;
	} else return false;
}

bool MarketAgent::Pay(MarketAgent *b, Sint64 amount, bool verbose) {
	if (m_money < amount) {
		if (verbose) {
			if (this == Pi::player) {
				Pi::Message(Lang::YOU_NOT_ENOUGH_MONEY);
			} else {
				Pi::Message(Lang::TRADER_NOT_ENOUGH_MONEY);
			}
		}
		return false;
	}
	b->m_money += amount;
	m_money -= amount;
	return true;
}
