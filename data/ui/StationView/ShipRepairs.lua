-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Translate = import("Translate")
local Engine = import("Engine")
local Game = import("Game")
local ShipDef = import("ShipDef")
local Rand = import("Rand")
local InfoGauge = import("ui/InfoGauge")
local InfoFace = import("ui/InfoFace")
local ModelSpinner = import("UI.Game.ModelSpinner")
local Character = import("Character")

local ui = Engine.ui
local t = Translate:GetTranslator()

local getRepairCost = function (percent, shipDef)
	-- repairing 1% hull damage costs 0.1% of ship price
	shipDef = shipDef or ShipDef[Game.player.shipId]
	return math.ceil(shipDef.basePrice * (percent * 0.1)) * 0.01
end

local getRepairMessage = function (damage, price)
	return string.interp(
		t("Repair {damage}% hull damage for {price}"), {
			damage = string.format('%.1f', damage),
			price = util.format_money(price)
		})
end

local shipRepairs = function (args)
	local messageLabel = ui:Label('<message>')
	local feedbackLabel = ui:Label('<feedback>')
	local repairButtons = ui:VBox(5)
	local repair1Label = ui:Label('Repair 1% hull damage for $.')
	local repairAllLabel = ui:Label('Repair all hull damage $$$.')
	local repair1Btn = ui:Button(ui:Expand('HORIZONTAL', repair1Label))
	local repairAllBtn = ui:Button(ui:Expand('HORIZONTAL', repairAllLabel))

	local cashLabel = ui:Label('<cash>')
	local integrityGauge = InfoGauge.New({
		formatter = function (v) return string.format('%.0f%%', v * 100); end,
		warningLevel = 0.5,
		criticalLevel = 0.2,
		levelAscending = false,
	})

	local damageAll, damage1, costRepairAll, costRepair1

	local update = function (feedbackText)
		local shipDef = ShipDef[Game.player.shipId]
		local stats = Game.player:GetStats()
		local hullPercent = stats.hullPercent

		cashLabel:SetText(string.interp(t("Cash: {money}"),
			{money = util.format_money(Game.player:GetMoney())}))
		integrityGauge:SetValue(hullPercent / 100)

		if hullPercent >= 100 then
			messageLabel:SetText(t("SHIP_IS_ALREADY_FULLY_REPAIRED"))
			feedbackLabel:SetText(feedbackText)
			repairButtons:Clear()
		else
			damageAll = 100 - hullPercent
			damage1 = math.min(damageAll, 1)
			costRepair1 = getRepairCost(damage1, shipDef)
			costRepairAll = getRepairCost(damageAll, shipDef)

			messageLabel:SetText(string.interp(t("Your hull is at {value}% integrity."),
				{value = string.format('%.1f', hullPercent)}))
			feedbackLabel:SetText(feedbackText)

			repair1Label:SetText(getRepairMessage(damage1, costRepair1))
			repairAllLabel:SetText(getRepairMessage(damageAll, costRepairAll))

			repairButtons:Clear()
			repairButtons:PackEnd(repair1Btn)
			if damageAll > damage1 then
				repairButtons:PackEnd(repairAllBtn)
			end
		end
	end

	local tryRepair = function (damage, price)
		if Game.player:GetMoney() >= price then
			local stats = Game.player:GetStats()
			Game.player:AddMoney(-price)
			Game.player:SetHullPercent(stats.hullPercent + damage)
			update('')
		else
			update(t("You don't have enough money for that option."))
		end
	end

	repair1Btn.onClick:Connect(function () tryRepair(damage1, costRepair1); end)
	repairAllBtn.onClick:Connect(function () tryRepair(damageAll, costRepairAll); end)

	-- initialise UI state
	update('')

	-- XXX need a better way of seeding this
	local station = Game.player:GetDockedWith()
	local rand = Rand.New(util.hash_random(station.seed .. '-repair-guy', 4294967296) - 1)
	local face = InfoFace.New(Character.New({ title = "Repair Guy" }, rand))

	local repairShop = ui:VBox(5):PackEnd({
		ui:Grid({1,1,1},1):SetCell(1, 0, face),
		messageLabel,
		repairButtons,
		feedbackLabel,
		ui:Expand('VERTICAL'),
	})

	local shipDef = ShipDef[Game.player.shipId]
	local shipView = ui:VBox(5):PackEnd({
		ui:Grid({1,2,1},1):SetCell(1, 0, ModelSpinner.New(ui, shipDef.modelName, Game.player:GetSkin())),
		ui:Expand('VERTICAL'),
		cashLabel,
		ui:Label(t("Hull Integrity")),
		integrityGauge,
	})

	return
		ui:Grid(2,1)
			:SetCell(0, 0, ui:Margin(20, 'RIGHT', repairShop))
			:SetCell(1, 0, ui:Margin(20, 'LEFT', shipView))
end

return shipRepairs
