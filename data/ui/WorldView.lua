-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Engine = import("Engine")
local Lang = import("Lang")
local Event = import("Event")

local ui = Engine.ui
local l = Lang.GetResource("ui-core");

local widget

local commsTable

ui.templates.WorldView = function (args)
	if widget then return widget end

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

Event.Register("onCommsMessage", function (m)
	local icon = m.priority and ui:Image(priorityIcons[m.priority]):SetHeightLines(1):SetTint(priorityColors[m.priority]) or ""
	local label = ui:Label(string.format("%s %s", m.from, m.message))
	commsTable:AddRow({ icon, label })
end)

Event.Register("onGameEnd", function ()
	widget = nil
end)
