-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Engine = import("Engine")
local Lang = import("Lang")
local Event = import("Event")

local ui = Engine.ui
local l = Lang.GetResource("ui-core");

local widget

local text

ui.templates.WorldView = function (args)
	if widget then return widget end

	text = ui:MultiLineText(""):SetFont("XSMALL")

	widget =
		ui:ColorBackground(0,0,0,0.5,
			text
		)

	return widget
end

Event.Register("onCommsMessage", function (m)
	text:AppendText(string.format("%s %s %s\n", m.priority, m.message, m.from))
end)

Event.Register("onGameEnd", function ()
    text:SetText("")
end)
