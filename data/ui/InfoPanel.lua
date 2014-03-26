-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Engine = import("Engine")
local Lang = import("Lang")
local Event = import("Event")
local Game = import("Game")

local ui = Engine.ui
local l = Lang.GetResource("ui-core");

local tabGroup

local widget

local commsTable
local commsIcon
local commsAnim

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

local function onCommsMessage (m)
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

	if tabGroup.collapsed and not commsAnim then
		commsAnim = ui:NewAnimation({
			widget = commsIcon,
			type = "IN_OUT",
			easing = "SINE",
			target = "OPACITY",
			duration = 1.0,
			continuous = true,
		})
		ui:Animate(commsAnim)
	end
end

local InfoPanel = {}

function InfoPanel.Get ()
	if widget then return widget end

	tabGroup =
		ui:TabGroup()
			:SetTransparent(true)
			:SetHeaderCollapsible(true)
			:SetFont("XSMALL")

	commsTable =
		ui:Table()
			:SetColumnSpacing(5)
			:SetRowSpacing(5)
			:SetRowAlignment("CENTER")

	commsIcon = ui:Image("icons/comms/normal_message.png"):SetHeightLines(1)
	local commsTab = tabGroup:NewTab(
		ui:HBox(3):PackEnd({ commsIcon, ui:Label("COMMS"):SetFont("HEADING_XSMALL") }),
		commsTable)

	tabGroup.onExpand:Connect(function ()
		if tabGroup.selectedTab == commsTab and commsAnim then
			commsAnim:Finish()
			commsAnim = nil
		end
	end)

	local grid =
		ui:Grid(1,5)
			:SetRow(0, {tabGroup})

	widget = grid

	Event.Deregister("onCommsMessage", onCommsMessage)
	Event.Register("onCommsMessage", onCommsMessage)

	return widget
end

Event.Register("onGameEnd", function ()
	widget = nil
	commsAnim = nil
end)

return InfoPanel
