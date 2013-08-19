-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Translate = import("Translate")
local Engine = import("Engine")
local Game = import("Game")
local EquipDef = import("EquipDef")
local Comms = import("Comms")

local InfoGauge = import("ui/InfoGauge")

local ui = Engine.ui
local t = Translate:GetTranslator()

local cargoIcon = {
	HYDROGEN =              "Hydrogen",
	LIQUID_OXYGEN =         "Liquid_Oxygen",
	METAL_ORE =             "Metal_ore",
	CARBON_ORE =            "Carbon_ore",
	METAL_ALLOYS =          "Metal_alloys",
	PLASTICS =              "Plastics",
	FRUIT_AND_VEG =         "Fruit_and_Veg",
	ANIMAL_MEAT =           "Animal_Meat",
	LIVE_ANIMALS =          "Live_Animals",
	LIQUOR =                "Liquor",
	GRAIN =                 "Grain",
	TEXTILES =              "Textiles",
	FERTILIZER =            "Fertilizer",
	WATER =                 "Water",
	MEDICINES =             "Medicines",
	CONSUMER_GOODS =        "Consumer_goods",
	COMPUTERS =             "Computers",
	ROBOTS =                "Robots",
	PRECIOUS_METALS =       "Precious_metals",
	INDUSTRIAL_MACHINERY =  "Industrial_machinery",
	FARM_MACHINERY =        "Farm_machinery",
	MINING_MACHINERY =      "Mining_machinery",
	AIR_PROCESSORS =        "Air_processors",
	SLAVES =                "Slaves",
	HAND_WEAPONS =          "Hand_weapons",
	BATTLE_WEAPONS =        "Battle_weapons",
	NERVE_GAS =             "Nerve_Gas",
	NARCOTICS =             "Narcotics",
	MILITARY_FUEL =         "Military_fuel",
	RUBBISH =               "Rubbish",
	RADIOACTIVES =          "Radioactive_waste",
}

local commodityMarket = function (args)
	local station = Game.player:GetDockedWith()

	local cargoTypes = {}
	for k,v in pairs(EquipDef) do
		if v.slot == "CARGO" and k ~= "NONE" then
			table.insert(cargoTypes, k)
		end
	end
	table.sort(cargoTypes)

	local stationCargo =
		ui:Table()
			:SetRowSpacing(5)
			:SetColumnSpacing(10)
			:SetHeadingRow({ "", "Name", "Price", "In stock" })
			:SetHeadingFont("LARGE")
			:SetMouseEnabled(true)

	local rowEquip = {}
	local row = 1
	for i=1,#cargoTypes do
		local e = cargoTypes[i]
		local icon = cargoIcon[e] and ui:Image("icons/goods/"..cargoIcon[e]..".png") or ""
		stationCargo:AddRow({ icon, t(e), string.format("%.02f", station:GetEquipmentPrice(e)), station:GetEquipmentStock(e) })
		rowEquip[row] = e
		row = row + 1
	end

	local shipCargo =
		ui:Table()
			:SetRowSpacing(5)
			:SetColumnSpacing(10)
			:SetHeadingRow({ "", "Name", "Amount" })
			:SetHeadingFont("LARGE")

	local function fillShipCargo ()
		shipCargo:ClearRows()
		for i=1,#cargoTypes do
			local e = cargoTypes[i]
			local n = Game.player:GetEquipCount("CARGO", e)
			if n > 0 then
				local icon = cargoIcon[e] and ui:Image("icons/goods/"..cargoIcon[e]..".png") or ""
				shipCargo:AddRow({ icon, t(e), n })
			end
		end
	end
	fillShipCargo()

	local cargoGauge = InfoGauge.New({
		formatter = function (v)
			local stats = Game.player:GetStats()
			return string.format("%d/%dt", stats.usedCargo, stats.maxCapacity-stats.usedCapacity+stats.usedCargo)
		end
	})
	local stats = Game.player:GetStats()
	cargoGauge:SetValue(stats.usedCargo/(stats.maxCapacity-stats.usedCapacity+stats.usedCargo))

	local cashLabel = ui:Label(string.format("$%.2f", Game.player:GetMoney()))

	stationCargo.onRowClicked:Connect( function (row)
		local e = rowEquip[row+1]

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

		fillShipCargo()

		stats = Game.player:GetStats()
		cargoGauge:SetValue(stats.usedCargo/(stats.maxCapacity-stats.usedCapacity+stats.usedCargo))
		cashLabel:SetText(string.format("$%.2f", Game.player:GetMoney()))
	end)


	return
		ui:Grid(2,1)
			:SetColumn(0, {
				ui:VBox():PackEnd({
					ui:Label("Available for purchase"):SetFont("HEADING_LARGE"),
					ui:Expand():SetInnerWidget(stationCargo),
				})
			})
			:SetColumn(1, {
				ui:VBox():PackEnd({
					ui:Label("In cargo hold"):SetFont("HEADING_LARGE"),
					ui:Expand():SetInnerWidget(shipCargo),
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

return commodityMarket
