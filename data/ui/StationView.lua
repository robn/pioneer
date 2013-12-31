-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Game = import("Game")
local Format = import("Format")
local Engine = import("Engine")
local ShipDef = import("ShipDef")
local Lang = import("Lang")
local Event = import("Event")

local TabGroup = import("ui/TabGroup")
local PlayerTabFooter = import("ui/PlayerTabFooter")
local InfoGauge = import("ui/InfoGauge")

local l = Lang.GetResource("ui-core")

local lobby           = import("StationView/Lobby")
local bulletinBoard   = import("StationView/BulletinBoard")
local commodityMarket = import("StationView/CommodityMarket")
local shipMarket      = import("StationView/ShipMarket")
local equipmentMarket = import("StationView/EquipmentMarket")
local shipRepairs     = import("StationView/ShipRepairs")
local police          = import("StationView/Police")

local ui = Engine.ui

local tabGroup
ui.templates.StationView = function (args)
	if tabGroup then
		tabGroup:SwitchFirst()
		return tabGroup.widget
	end

	tabGroup = TabGroup.New()

	tabGroup:AddTab({ id = "lobby",           title = l.LOBBY,            icon = "Info",      template = lobby           })
	tabGroup:AddTab({ id = "bulletinBoard",   title = l.BULLETIN_BOARD,   icon = "Clipboard", template = bulletinBoard   })
	tabGroup:AddTab({ id = "commodityMarket", title = l.COMMODITY_MARKET, icon = "Cart",      template = commodityMarket })
	tabGroup:AddTab({ id = "shipMarket",      title = l.SHIP_MARKET,      icon = "Car",       template = shipMarket      })
	tabGroup:AddTab({ id = "equipmentMarket", title = l.EQUIPMENT_MARKET, icon = "Radio",     template = equipmentMarket })
	tabGroup:AddTab({ id = "shipRepairs",     title = l.SHIP_REPAIRS,     icon = "Tools",     template = shipRepairs     })
	tabGroup:AddTab({ id = "police",          title = l.POLICE,           icon = "Shield",    template = police          })

	tabGroup:SetFooter(PlayerTabFooter.New())

	return tabGroup.widget
end

Event.Register("onGameEnd", function ()
	tabGroup = nil
end)
