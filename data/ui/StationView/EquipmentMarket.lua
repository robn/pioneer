-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Translate = import("Translate")
local Engine = import("Engine")
local Game = import("Game")
local Comms = import("Comms")

local InfoGauge = import("ui/InfoGauge")

local EquipmentTableWidgets = import("EquipmentTableWidgets")

local ui = Engine.ui
local t = Translate:GetTranslator()


local equipmentMarket = function (args)
	local station = Game.player:GetDockedWith()

	local cargoGauge = InfoGauge.New({
		formatter = function (v)
			local stats = Game.player:GetStats()
			return string.format("%d/%dt", stats.usedCargo, stats.maxCapacity-stats.usedCapacity+stats.usedCargo)
		end
	})

	local cashLabel = ui:Label("")

	local function updateStats ()
		local stats = Game.player:GetStats()
		cargoGauge:SetValue(stats.usedCargo/(stats.maxCapacity-stats.usedCapacity+stats.usedCargo))
		cashLabel:SetText(string.format("$%.2f", Game.player:GetMoney()))
	end
	updateStats()

	local stationTable, shipTable = EquipmentTableWidgets.Pair({
		station = station,

		isValidSlot = function (slot) return slot ~= "CARGO" end,

		onBuy = function (e)
			if station:GetEquipmentStock(e) <= 0 then
				Comms.message(t("ITEM_IS_OUT_OF_STOCK"))
				return
			end

			local stats = Game.player:GetStats()

			-- XXX check slot capacity

			if stats.freeCapacity <= 0 then
				Comms.Message(t("SHIP_IS_FULLY_LADEN"))
				return
			end

			if Game.player:GetMoney() < station:GetEquipmentPrice(e) then
				Comms.Message(t("YOU_NOT_ENOUGH_MONEY"))
				return
			end

			assert(Game.player:AddEquip(e) == 1)
			Game.player:AddMoney(-station:GetEquipmentPrice(e))
			-- XXX remove from station stock

			updateStats()
		end,

		onSell = function (e)
			Game.player:RemoveEquip(e)
			Game.player:AddMoney(station:GetEquipmentPrice(e))
			-- XXX add to station stock

			updateStats()
		end,
	})

	return
		ui:Grid(2,1)
			:SetColumn(0, {
				ui:VBox():PackEnd({
					ui:Label("Available for purchase"):SetFont("HEADING_LARGE"),
					ui:Expand():SetInnerWidget(stationTable),
				})
			})
			:SetColumn(1, {
				ui:VBox():PackEnd({
					ui:Label("In cargo hold"):SetFont("HEADING_LARGE"),
					ui:Expand():SetInnerWidget(shipTable),
					ui:HBox():PackEnd({
						"Cash: ",
						cashLabel
					}),
					ui:HBox():PackEnd({
						"Cargo space: ",
						cargoGauge
					})
				})
			})
end

return equipmentMarket
