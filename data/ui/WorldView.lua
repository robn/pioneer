-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Engine = import("Engine")
local Lang = import("Lang")
local Event = import("Event")
local Game = import("Game")

local ui = Engine.ui
local l = Lang.GetResource("ui-core");

local widget

local commsTable

ui.templates.WorldView = function (args)
	if widget then
		commsTable:ScrollToBottom()
		return widget
	end

	local tg =
		ui:TabGroup()
			:SetTransparent(true)
			:SetHeaderCollapsible(true)
			:SetFont("XSMALL")

	commsTable =
		ui:Table()
			:SetColumnSpacing(5)
			:SetRowSpacing(5)
			:SetRowAlignment("CENTER")
	tg:NewTab(ui:Label("COMMS"):SetFont("HEADING_XSMALL"), commsTable)

	local grid =
		ui:Grid(1,5)
			:SetRow(0, {tg})

	widget = grid
	return widget
end

local priorityIcons = {
	normal    = "icons/comms/normal_message.png",
	important = "icons/comms/important_message.png",
	urgent    = "icons/comms/urgent_message.png",
}

local priorityColors = {
	normal    = { r = 1.0, g = 1.0, b = 1.0 },
	important = { r = 1.0, g = 0.6, b = 0.0 },
	urgent    = { r = 1.0, g = 0.0, b = 0.0 },
}

local fromColor   = { r = 1.0, g = 1.0, b = 0.4 }
local targetColor = { r = 0.4, g = 1.0, b = 0.4 }

Event.Register("onCommsMessage", function (m)
	local icon = m.priority and ui:Image(priorityIcons[m.priority]):SetHeightLines(1):SetTint(priorityColors[m.priority]) or ""
	local text = ui:Label(m.message)

	local info
	if m.from then
		local from = ui:Label(m.from):SetColor(fromColor)
		if m.target then
			from:SetColor(targetColor)
			from.onClick:Connect(function ()
				if m.target:exists() then
					if m.target:IsDynamic() then
						Game.player:SetCombatTarget(m.target)
					else
						Game.player:SetNavTarget(m.target)
					end
				end
			end)
		end
		info = ui:HBox(3):PackEnd({ from, text })
	else
		info = text
	end

	commsTable:AddRow({ icon, info })
	commsTable:ScrollToBottom()
end)

Event.Register("onGameEnd", function ()
	widget = nil
end)
