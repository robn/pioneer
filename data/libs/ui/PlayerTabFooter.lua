-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Lang = import("Lang")
local Engine = import("Engine")
local Game = import("Game")

local l = Lang.GetResource("ui-core")

local ui = Engine.ui

local PlayerTabFooter = {}

function PlayerTabFooter.New ()
	local player = Game.player

	local cashLabel = ui:NumberLabel("MONEY")
	cashLabel:Bind("value", player, "cash")

	local cargoGauge = ui:Gauge()
	local cargoUsedLabel = ui:Label("")
	local cargoFreeLabel = ui:Label("")
	local function cargoUpdate ()
		cargoGauge:SetUpperValue(player.totalCargo)
		cargoGauge:SetValue(player.usedCargo)
		cargoUsedLabel:SetText(string.interp(l.CARGO_T_USED, { amount = player.usedCargo }))
		cargoFreeLabel:SetText(string.interp(l.CARGO_T_FREE, { amount = player.totalCargo-player.usedCargo }))
	end
	player:Connect("usedCargo", cargoUpdate)
	player:Connect("totalCargo", cargoUpdate)
	cargoUpdate()

	local cabinGauge = ui:Gauge()
	local cabinUsedLabel = ui:Label("")
	local cabinFreeLabel = ui:Label("")
	local function cabinUpdate ()
		cabinGauge:SetUpperValue(player.totalCabins)
		cabinGauge:SetValue(player.usedCabins)
		cabinUsedLabel:SetText(string.interp(l.CABIN_USED, { amount = player.usedCabins }))
		cabinFreeLabel:SetText(string.interp(l.CABIN_FREE, { amount = player.totalCabins-player.usedCabins }))
	end
	player:Connect("usedCabins", cabinUpdate)
	player:Connect("totalCabins", cabinUpdate)
	cabinUpdate()

	local footer =
		ui:Margin(15, "TOP",
			ui:Margin(5, "VERTICAL",
				ui:Grid({15,30,30,15},1):SetRow(0, {
					ui:Margin(10, "HORIZONTAL",
						ui:HBox():PackEnd({
							l.CASH..": ",
							cashLabel,
						})
					),
					ui:Margin(10, "HORIZONTAL",
						ui:HBox(10):PackEnd({
							ui:Align("MIDDLE",
								ui:HBox(10):PackEnd({
									l.CARGO..":",
									cargoGauge,
								})
							),
							ui:VBox():PackEnd({
								cargoUsedLabel,
								cargoFreeLabel,
							}):SetFont("XSMALL"),
						})
					),
					ui:Margin(10, "HORIZONTAL",
						ui:HBox(10):PackEnd({
							ui:Align("MIDDLE",
								ui:HBox(10):PackEnd({
									l.CABINS..":",
									cabinGauge,
								})
							),
							ui:VBox():PackEnd({
								cabinUsedLabel,
								cabinFreeLabel,
							}):SetFont("XSMALL"),
						})
					),
					ui:Margin(10, "HORIZONTAL",
						ui:Align("RIGHT",
							l.LEGAL_STATUS..": "..l.CLEAN
						)
					),
				})
			)
		)

	local self = {
		widget = footer
	}
	setmetatable(self, {
		__index = PlayerTabFooter,
		class = "UI.PlayerTabFooter",
	})

	return self
end

return PlayerTabFooter
