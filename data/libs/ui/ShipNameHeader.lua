-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Lang = import("Lang")
local Engine = import("Engine")
local Game = import("Game")

local ui = Engine.ui

local l = Lang.GetResource("ui-core")

local shipClassString = {
	light_cargo_shuttle        = l.LIGHT_CARGO_SHUTTLE,
	light_courier              = l.LIGHT_COURIER,
	light_fighter              = l.LIGHT_FIGHTER,
	light_freighter            = l.LIGHT_FREIGHTER,
	light_passenger_shuttle    = l.LIGHT_PASSENGER_SHUTTLE,
	light_passenger_transport  = l.LIGHT_PASSENGER_TRANSPORT,
	medium_cargo_shuttle       = l.MEDIUM_CARGO_SHUTTLE,
	medium_courier             = l.MEDIUM_COURIER,
	medium_fighter             = l.MEDIUM_FIGHTER,
	medium_freighter           = l.MEDIUM_FREIGHTER,
	medium_passenger_shuttle   = l.MEDIUM_PASSENGER_SHUTTLE,
	medium_passenger_transport = l.MEDIUM_PASSENGER_TRANSPORT,
	heavy_cargo_shuttle        = l.HEAVY_CARGO_SHUTTLE,
	heavy_courier              = l.HEAVY_COURIER,
	heavy_fighter              = l.HEAVY_FIGHTER,
	heavy_freighter            = l.HEAVY_FREIGHTER,
	heavy_passenger_shuttle    = l.HEAVY_PASSENGER_SHUTTLE,
	heavy_passenger_transport  = l.HEAVY_PASSENGER_TRANSPORT,

	unknown                    = "",
}

local function manufacturerIcon (manufacturer)
	return manufacturer ~= "unknown"
		and ui:Image("icons/manufacturer/"..manufacturer..".png", { "PRESERVE_ASPECT" })
		or ui:Margin(32)
end

local ShipNameHeader = {}

function ShipNameHeader.New (def)
	local header = ui:HBox():PackEnd({
		ui:Align("LEFT",
			ui:VBox():PackEnd({
				ui:Label(def.name):SetFont("HEADING_LARGE"),
				ui:Label(shipClassString[def.shipClass]):SetFont("HEADING_SMALL"),
			})
		),
		ui:Expand("HORIZONTAL", ui:Align("RIGHT", manufacturerIcon(def.manufacturer))),
	})

	local self = {
		widget = header,
	}
	setmetatable(self, {
		__index = ShipNameHeader,
		class = "UI.ShipNameHeader",
	})

	return self
end

return ShipNameHeader
